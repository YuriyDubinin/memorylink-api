#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <string>

class S3Client {
public:
    S3Client(const std::string& access_key,
             const std::string& secret_key,
             const std::string& endpoint,
             const std::string& region);

    bool UploadFile(const std::string& bucket, const std::string& key, const std::string& data);
    bool DeleteFile(const std::string& bucket, const std::string& key);

    std::string DownloadFile(const std::string& bucket, const std::string& key);

private:
    std::string access_key_;
    std::string secret_key_;
    std::string endpoint_;
    std::string region_;

    std::mutex client_mutex_;

    std::string BuildUrl_(const std::string& bucket, const std::string& key) const;
};

using S3ClientPtr = std::shared_ptr<S3Client>;
S3ClientPtr GetS3Client();
void        InitS3Client(const std::string& access_key,
                         const std::string& secret_key,
                         const std::string& endpoint,
                         const std::string& region);
