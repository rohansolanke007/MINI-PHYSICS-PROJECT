#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include "SimulationState.h"

// FileHandler
// ----------------------------------------------------------------
// Responsible for all file I/O: saving a SimulationState to a text
// file, and loading one back. Kept as a separate class (rather than
// folding this logic into World) so that File I/O concerns stay
// decoupled from simulation logic - World doesn't need to know the
// file format at all.
//
// File format (plain text, one simulation per file):
//   SIMULATION
//   TIME <double>
//   OBJECT_COUNT <n>
//   OBJECT <id> <STATIC|DYNAMIC> <SHAPE_TYPE> <param1> [param2] <mass> <posX> <posY> <velX> <velY>
//   ... (repeated n times)
//   END
//
// OOP concepts demonstrated:
//   - File streams (ifstream/ofstream), text file I/O
//   - Custom exceptions on failure (FileException, InvalidSimulationException)
//   - Static methods (this class holds no state of its own)
// ----------------------------------------------------------------
class FileHandler {
public:
    static void saveToFile(const SimulationState& state, const std::string& filename);
    static SimulationState loadFromFile(const std::string& filename);
};

#endif // FILE_HANDLER_H
