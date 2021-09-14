/*
 * Copyright (c) 1995-2021, Nikolay Pultsin <geometer@geometer.name>
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __KE_UTIL_RAPIDJSON_H__
#define __KE_UTIL_RAPIDJSON_H__

#include <string>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

namespace KE::Util::rapidjson {

inline std::string getString(const ::rapidjson::Value &doc, const std::string &key) {
	if (doc.IsObject() && doc.HasMember(key.c_str())) {
		const auto &obj = doc[key.c_str()];
		if (obj.IsString()) {
			return std::string(obj.GetString(), obj.GetStringLength());
		}
	}
	return "";
}

inline std::string docToString(const ::rapidjson::Document &doc) {
	::rapidjson::StringBuffer buffer;
	buffer.Clear();
	::rapidjson::Writer<::rapidjson::StringBuffer> writer(buffer);
	writer.SetMaxDecimalPlaces(5);
	doc.Accept(writer);
	return std::string(buffer.GetString(), buffer.GetSize());
}

}

#endif /* __KE_UTIL_RAPIDJSON_H__ */
