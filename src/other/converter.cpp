#include <iostream>
#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "../util/rapidjson.h"
#include "../math/diagram/Diagram.h"
#include "../math/knot/Knot.h"

int main(int argc, const char **argv) {
	rapidjson::Document doc;
	{
		std::ifstream is(argv[1]);
		rapidjson::IStreamWrapper wrapper(is);
		doc.ParseStream(wrapper);
		is.close();
	}

	const std::string type = KE::Util::rapidjson::get_string(doc, "type");
	if (type == "link") {
		KE::ThreeD::Knot knot(doc);
		doc = knot.serialize();
	} else if (type == "diagram") {
		KE::TwoD::Diagram diagram(doc);
		doc = diagram.serialize();
	} else {
		throw std::runtime_error("Unknown file type");
	}

	{
		std::ofstream os(argv[1]);
		rapidjson::OStreamWrapper wrapper(os);
		rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
		writer.SetMaxDecimalPlaces(5);
		doc.Accept(writer);
		os.close();
	}

	return 0;
}
