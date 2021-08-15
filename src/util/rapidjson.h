#ifndef __RAPIDJSON_H__
#define __RAPIDJSON_H__

#include <sstream>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

namespace KE { namespace Util { namespace rapidjson {

inline std::string getString(const ::rapidjson::Document &doc, const std::string &key) {
	if (doc.HasMember(key.c_str())) {
		const auto &obj = doc[key.c_str()];
		if (obj.IsString()) {
			return std::string(obj.GetString(), obj.GetStringLength());
		}
	}
	return "";
}

inline std::string docToString(const ::rapidjson::Document &doc) {
	std::stringstream os;
	::rapidjson::OStreamWrapper wrapper(os);
	::rapidjson::Writer<::rapidjson::OStreamWrapper> writer(wrapper);
	writer.SetMaxDecimalPlaces(5);
	doc.Accept(writer);
	return os.str();
}

}}}

#endif /* __RAPIDJSON_H__ */
