#include "default_request.h"

namespace validate {
    bool file_request(const httplib::Request& req, ApiResponse& api_response) {
        if (!req.is_multipart_form_data()) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Expected multipart/form-data";
            return false;
        }

        if (req.form.files.empty()) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "No files uploaded";
            return false;
        }

        return true;
    }

} // namespace validate