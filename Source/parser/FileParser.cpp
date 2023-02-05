#include "FileParser.h"
#include "../shape/Line.h"

FileParser::FileParser() {}

void FileParser::parse(juce::String extension, std::unique_ptr<juce::InputStream> stream) {
	object = nullptr;
	camera = nullptr;
	svg = nullptr;
	
	if (extension == ".obj") {
		object = std::make_unique<WorldObject>(stream->readEntireStreamAsString().toStdString());
		camera = std::make_unique<Camera>(1.0, 0, 0, 0.0);
		camera->findZPos(*object);
	} else if (extension == ".svg") {
		svg = std::make_unique<SvgParser>(stream->readEntireStreamAsString());
	}
}

std::vector<std::unique_ptr<Shape>> FileParser::next() {
	if (object != nullptr && camera != nullptr) {
		return camera->draw(*object);
	} else if (svg != nullptr) {
		return svg->draw();
	}
	auto tempShapes = std::vector<std::unique_ptr<Shape>>();
	tempShapes.push_back(std::make_unique<Line>(0.0, 0.0, 1.0, 1.0));
	return tempShapes;
}

bool FileParser::isActive() {
	return active;
}

void FileParser::disable() {
	active = false;
}

void FileParser::enable() {
	active = true;
}
