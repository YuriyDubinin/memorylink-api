#include "s3_client.h"

namespace {
    Aws::SDKOptions sdk_options;
    S3ClientPtr     global_s3_client = nullptr;

    constexpr std::size_t MULTIPART_THRESHOLD = 50 * 1024 * 1024; // 50 MB
    constexpr std::size_t PART_SIZE           = 10 * 1024 * 1024; // 10 MB
} // namespace

S3Client::S3Client(const std::string& access_key,
                   const std::string& secret_key,
                   const std::string& endpoint,
                   const std::string& region) {
    Aws::InitAPI(sdk_options);

    Aws::Client::ClientConfiguration config;
    config.region           = region;
    config.endpointOverride = endpoint;

    client_ = std::make_shared<Aws::S3::S3Client>(
        Aws::Auth::AWSCredentials(access_key, secret_key),
        config,
        Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never,
        false);

    std::cout << "[S3Client]: initialized: " << endpoint << "\n";
}

S3Client::~S3Client() {
    client_.reset();
    Aws::ShutdownAPI(sdk_options);
}

bool S3Client::Upload(const std::string& bucket,
                      const std::string& key,
                      const std::string& file_path,
                      const std::string& content_type) {
    std::lock_guard<std::mutex> lock(client_mutex_);

    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "[S3Client][UPLOAD] Cannot open file: " << file_path << "\n";
        return false;
    }

    std::size_t file_size = static_cast<std::size_t>(file.tellg());
    file.close();

    if (file_size < MULTIPART_THRESHOLD) {
        return UploadSingle(bucket, key, file_path, content_type);
    }
    return UploadMultipart(bucket, key, file_path, content_type);
}

bool S3Client::UploadSingle(const std::string& bucket,
                            const std::string& key,
                            const std::string& file_path,
                            const std::string& content_type) {
    auto stream = Aws::MakeShared<Aws::FStream>(
        "PutObject", file_path.c_str(), std::ios::binary | std::ios::in);

    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(bucket);
    request.SetKey(key);
    request.SetContentType(content_type);
    request.SetBody(stream);

    auto outcome = client_->PutObject(request);
    if (!outcome.IsSuccess()) {
        std::cerr << "[S3Client][UPLOAD] " << outcome.GetError().GetMessage() << "\n";
        return false;
    }
    return true;
}

bool S3Client::UploadMultipart(const std::string& bucket,
                               const std::string& key,
                               const std::string& file_path,
                               const std::string& content_type) {
    Aws::S3::Model::CreateMultipartUploadRequest create;
    create.SetBucket(bucket);
    create.SetKey(key);
    create.SetContentType(content_type);

    auto create_outcome = client_->CreateMultipartUpload(create);
    if (!create_outcome.IsSuccess()) {
        return false;
    }

    const auto    upload_id = create_outcome.GetResult().GetUploadId();
    std::ifstream file(file_path, std::ios::binary);

    std::vector<Aws::S3::Model::CompletedPart> completed_parts;
    int                                        part_number = 1;

    while (file.good()) {
        std::vector<char> buffer(PART_SIZE);
        file.read(buffer.data(), buffer.size());
        auto bytes = file.gcount();
        if (bytes <= 0)
            break;

        auto stream = Aws::MakeShared<Aws::StringStream>("UploadPart");
        stream->write(buffer.data(), bytes);

        Aws::S3::Model::UploadPartRequest upload;
        upload.SetBucket(bucket);
        upload.SetKey(key);
        upload.SetUploadId(upload_id);
        upload.SetPartNumber(part_number);
        upload.SetContentLength(bytes);
        upload.SetBody(stream);

        auto outcome = client_->UploadPart(upload);
        if (!outcome.IsSuccess()) {
            Aws::S3::Model::AbortMultipartUploadRequest abort;
            abort.SetBucket(bucket);
            abort.SetKey(key);
            abort.SetUploadId(upload_id);
            client_->AbortMultipartUpload(abort);
            return false;
        }

        Aws::S3::Model::CompletedPart part;
        part.SetPartNumber(part_number++);
        part.SetETag(outcome.GetResult().GetETag());
        completed_parts.push_back(part);
    }

    Aws::S3::Model::CompletedMultipartUpload completed;
    completed.SetParts(completed_parts);

    Aws::S3::Model::CompleteMultipartUploadRequest complete;
    complete.SetBucket(bucket);
    complete.SetKey(key);
    complete.SetUploadId(upload_id);
    complete.SetMultipartUpload(completed);

    return client_->CompleteMultipartUpload(complete).IsSuccess();
}

bool S3Client::Download(const std::string& bucket,
                        const std::string& key,
                        const std::string& file_path) {
    std::lock_guard<std::mutex> lock(client_mutex_);

    Aws::S3::Model::GetObjectRequest request;
    request.SetBucket(bucket);
    request.SetKey(key);

    auto outcome = client_->GetObject(request);
    if (!outcome.IsSuccess()) {
        std::cerr << "[S3Client][DOWNLOAD] " << outcome.GetError().GetMessage() << "\n";
        return false;
    }

    std::ofstream output(file_path, std::ios::binary);
    output << outcome.GetResult().GetBody().rdbuf();
    return true;
}

bool S3Client::Delete(const std::string& bucket, const std::string& key) {
    std::lock_guard<std::mutex> lock(client_mutex_);

    Aws::S3::Model::DeleteObjectRequest request;
    request.SetBucket(bucket);
    request.SetKey(key);

    return client_->DeleteObject(request).IsSuccess();
}

S3ClientPtr GetS3Client() {
    return global_s3_client;
}

void InitS3Client(const std::string& access_key,
                  const std::string& secret_key,
                  const std::string& endpoint,
                  const std::string& region) {
    if (!global_s3_client) {
        global_s3_client = std::make_shared<S3Client>(access_key, secret_key, endpoint, region);
    }
}
