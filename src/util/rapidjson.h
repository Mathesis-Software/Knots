#ifndef __RAPIDJSON_H__
#define __RAPIDJSON_H__

#include <string>

#include <rapidjson/document.h>

namespace KE { namespace Util { namespace rapidjson {

inline std::string get_string(const ::rapidjson::Document &doc, const std::string &key) {
	if (doc.HasMember(key.c_str())) {
		const auto &obj = doc[key.c_str()];
		if (obj.IsString()) {
			return std::string(obj.GetString(), obj.GetStringLength());
		}
	}
	return "";
}

}}}

#endif /* __RAPIDJSON_H__ */
