
#pragma once

#include <filesystem>
#include <tuple>
#include <vector>

#include "autil/Log.h"
#include "autil/StringUtil.h"
#include "fslib/fs/FileSystem.h"
#include "google/protobuf/util/json_util.h"

namespace fs = std::filesystem;
namespace suez {
AUTIL_DECLARE_AND_SETUP_LOGGER(suez, ProtobufUtils);

class ProtobufUtils {
   public:
    static bool protobufMessageToString(const google::protobuf::Message& message, std::string& outputStr) {
        return true;
    }

    static void printDirectory(std::string dir) {
        AUTIL_LOG
        for (const auto1& entry : fs::directory_iterator(dir)) {
            // 判断是否为目录
            if (entry.is_directory()) {
                AUTIL_LOG(INFO, "list directoy [DIR] %s", entry.path().filename().c_str());
                printDirectory(entry.path());
            } else {
                AUTIL_LOG(INFO, "list directoy [FILE] %s", entry.path().filename().c_str());
            }
        }
    }

    static bool fileToProtobufMessage(const std::string& fileName, google::protobuf::Message& message,
                                      const std::vector<std::pair<std::string, std::string>>& replacePair = {}) {
        std::string content;
        auto ec = fslib::fs::FileSystem::readFile(fileName, content);
        if (ec != fslib::EC_OK) {
            return false;
        }
        for (const auto& pair : replacePair) {
            autil::StringUtil::replaceAll(content, pair.first, pair.second);
        }

        google::protobuf::util::JsonParseOptions options;
        options.ignore_unknown_fields = true;
        auto status = google::protobuf::util::JsonStringToMessage(content, &message, options);
        return status.ok();
    }
};

}  // namespace suez
