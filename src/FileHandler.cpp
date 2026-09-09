#include "FileHandler.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>

void FileHandler::saveToFile(const SimulationState& state, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileException("Could not open file for writing: " + filename);
    }

    file << "SIMULATION\n";
    file << "TIME " << state.simulationTime << "\n";
    file << "OBJECT_COUNT " << state.records.size() << "\n";

    for (const auto& rec : state.records) {
        file << "OBJECT " << rec.id << " "
             << (rec.isStatic ? "STATIC" : "DYNAMIC") << " "
             << rec.shapeType << " ";
        for (double p : rec.shapeParams) {
            file << p << " ";
        }
        file << rec.mass << " "
             << rec.position.x << " " << rec.position.y << " "
             << rec.velocity.x << " " << rec.velocity.y << "\n";
    }

    file << "END\n";

    if (file.fail()) {
        throw FileException("An error occurred while writing to file: " + filename);
    }
    file.close();
}

SimulationState FileHandler::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileException("Could not open file for reading: " + filename);
    }

    SimulationState state;
    std::string tag;

    file >> tag;
    if (tag != "SIMULATION") {
        throw InvalidSimulationException("File does not start with SIMULATION header: " + filename);
    }

    file >> tag; // "TIME"
    if (tag != "TIME") throw InvalidSimulationException("Expected TIME field in file.");
    file >> state.simulationTime;

    file >> tag; // "OBJECT_COUNT"
    if (tag != "OBJECT_COUNT") throw InvalidSimulationException("Expected OBJECT_COUNT field in file.");
    int count;
    file >> count;
    if (count < 0) throw InvalidSimulationException("Corrupted file: negative object count.");

    for (int i = 0; i < count; ++i) {
        file >> tag; // "OBJECT"
        if (tag != "OBJECT") {
            throw InvalidSimulationException("Corrupted simulation data: expected OBJECT record #" + std::to_string(i));
        }

        ObjectRecord rec;
        std::string kindStr, shapeStr;
        file >> rec.id >> kindStr >> shapeStr;

        rec.isStatic = (kindStr == "STATIC");
        rec.shapeType = shapeStr;

        int paramCount = 0;
        if (shapeStr == "CIRCLE") paramCount = 1;
        else if (shapeStr == "RECTANGLE") paramCount = 2;
        else if (shapeStr == "TRIANGLE") paramCount = 2;
        else throw InvalidSimulationException("Corrupted simulation data: unknown shape type '" + shapeStr + "'");

        for (int p = 0; p < paramCount; ++p) {
            double val;
            file >> val;
            rec.shapeParams.push_back(val);
        }

        file >> rec.mass >> rec.position.x >> rec.position.y >> rec.velocity.x >> rec.velocity.y;

        if (file.fail()) {
            throw InvalidSimulationException("Corrupted simulation data while reading OBJECT record #" + std::to_string(i));
        }

        state.records.push_back(rec);
    }

    file >> tag;
    if (tag != "END") {
        throw InvalidSimulationException("File missing END marker - possibly corrupted: " + filename);
    }

    file.close();
    return state;
}
