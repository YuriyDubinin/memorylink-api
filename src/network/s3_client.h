#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/AbortMultipartUploadRequest.h>
#include <aws/s3/model/CompleteMultipartUploadRequest.h>
#include <aws/s3/model/CreateMultipartUploadRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/UploadPartRequest.h>

class S3Client {
public:
    S3Client(const std::string& access_key,
             const std::string& secret_key,
             const std::string& endpoint,
             const std::string& region);

    ~S3Client();

    // Upload ANY file (photo / video / binary / large files)
    bool Upload(const std::string& bucket,
                const std::string& key,
                const std::string& file_path,
                const std::string& content_type = "application/octet-stream");

    bool Download(const std::string& bucket, const std::string& key, const std::string& file_path);

    bool Delete(const std::string& bucket, const std::string& key);

    bool UploadFromMemory(const std::string&       bucket,
                          const std::string&       key,
                          const std::vector<char>& data,
                          const std::string&       content_type);

private:
    bool UploadSingle(const std::string& bucket,
                      const std::string& key,
                      const std::string& file_path,
                      const std::string& content_type);

    bool UploadMultipart(const std::string& bucket,
                         const std::string& key,
                         const std::string& file_path,
                         const std::string& content_type);

private:
    std::mutex                         client_mutex_;
    std::shared_ptr<Aws::S3::S3Client> client_;
};

using S3ClientPtr = std::shared_ptr<S3Client>;

S3ClientPtr GetS3Client();
void        InitS3Client(const std::string& access_key,
                         const std::string& secret_key,
                         const std::string& endpoint,
                         const std::string& region);
