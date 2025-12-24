#include "s3_client.h"

S3Client::S3Client(const std::string& access_key,
                   const std::string& secret_key,
                   const std::string& endpoint,
                   const std::string& region)
    : access_key_(access_key), secret_key_(secret_key), endpoint_(endpoint), region_(region) {
    std::cout << "[S3Client]: initialized with endpoint: " << endpoint_ << "\n";
}

std::string S3Client::BuildUrl_(const std::string& bucket, const std::string& key) const {
    return endpoint_ + "/" + bucket + "/" + key;
}

bool S3Client::UploadFile(const std::string& bucket,
                          const std::string& key,
                          const std::string& data) {
    std::lock_guard<std::mutex> lock(client_mutex_);
    std::string                 url = BuildUrl_(bucket, key);
    std::cout << "[S3Client]: UPLOAD: URL: " << url << ", size: " << data.size() << " bytes\n";
    return true;
}

std::string S3Client::DownloadFile(const std::string& bucket, const std::string& key) {
    std::lock_guard<std::mutex> lock(client_mutex_);
    std::string                 url = BuildUrl_(bucket, key);
    std::cout << "[S3Client]: DOWNLOAD: URL: " << url << "\n";
    return "file content";
}

bool S3Client::DeleteFile(const std::string& bucket, const std::string& key) {
    std::lock_guard<std::mutex> lock(client_mutex_);
    std::string                 url = BuildUrl_(bucket, key);
    std::cout << "[DELETE]: URL: " << url << "\n";
    return true;
}

namespace {
    S3ClientPtr global_s3_client = nullptr;
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
