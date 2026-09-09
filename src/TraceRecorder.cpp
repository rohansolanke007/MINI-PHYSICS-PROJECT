#include "TraceRecorder.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>

TraceRecorder::TraceRecorder(double timeStep, const Vector2D& boundaryMin, const Vector2D& boundaryMax)
    : timeStep(timeStep), boundaryMin(boundaryMin), boundaryMax(boundaryMax) {}

void TraceRecorder::recordFrame(const World& world) {
    FrameRecord frame;
    frame.state = SimulationState::captureFrom(world);
    frame.collisions = world.getRecentCollisionsSnapshot();
    frames.push_back(frame);
}

void TraceRecorder::clear() {
    frames.clear();
}

// Small local helper: JSON only needs numbers here (no user-supplied
// strings besides fixed enum-like values such as "CIRCLE"/"DYNAMIC"),
// so no escaping logic is required - keeping this simple and honest
// rather than pulling in a JSON library for a student project.
static std::string vecToJSON(const Vector2D& v) {
    std::ostringstream out;
    out << "{\"x\":" << v.x << ",\"y\":" << v.y << "}";
    return out.str();
}

void TraceRecorder::exportToJSON(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileException("Could not open file for writing trace: " + filename);
    }

    file << "{\n";
    file << "  \"timeStep\": " << timeStep << ",\n";
    file << "  \"boundaryMin\": " << vecToJSON(boundaryMin) << ",\n";
    file << "  \"boundaryMax\": " << vecToJSON(boundaryMax) << ",\n";
    file << "  \"frameCount\": " << frames.size() << ",\n";
    file << "  \"frames\": [\n";

    for (size_t f = 0; f < frames.size(); ++f) {
        const auto& frame = frames[f];
        file << "    {\n";
        file << "      \"time\": " << frame.state.simulationTime << ",\n";
        file << "      \"objects\": [\n";

        const auto& records = frame.state.records;
        for (size_t i = 0; i < records.size(); ++i) {
            const auto& rec = records[i];
            file << "        {"
                 << "\"id\":" << rec.id << ","
                 << "\"isStatic\":" << (rec.isStatic ? "true" : "false") << ","
                 << "\"shapeType\":\"" << rec.shapeType << "\","
                 << "\"shapeParams\":[";
            for (size_t p = 0; p < rec.shapeParams.size(); ++p) {
                file << rec.shapeParams[p];
                if (p + 1 < rec.shapeParams.size()) file << ",";
            }
            file << "],"
                 << "\"mass\":" << rec.mass << ","
                 << "\"pos\":" << vecToJSON(rec.position) << ","
                 << "\"vel\":" << vecToJSON(rec.velocity)
                 << "}";
            if (i + 1 < records.size()) file << ",";
            file << "\n";
        }
        file << "      ],\n";

        file << "      \"collisions\": [";
        for (size_t c = 0; c < frame.collisions.size(); ++c) {
            file << "{\"idA\":" << frame.collisions[c].idA
                 << ",\"idB\":" << frame.collisions[c].idB << "}";
            if (c + 1 < frame.collisions.size()) file << ",";
        }
        file << "]\n";

        file << "    }";
        if (f + 1 < frames.size()) file << ",";
        file << "\n";
    }

    file << "  ]\n";
    file << "}\n";

    if (file.fail()) {
        throw FileException("An error occurred while writing trace file: " + filename);
    }
    file.close();
}
