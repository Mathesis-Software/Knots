/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Author: Nikolay Pultsin <geometer@geometer.name>
 */

#ifndef __RAPIDJSON_H__
#define __RAPIDJSON_H__

#include <sstream>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

namespace KE { namespace Util { namespace rapidjson {

inline std::string getString(const ::rapidjson::Value &doc, const std::string &key) {
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
