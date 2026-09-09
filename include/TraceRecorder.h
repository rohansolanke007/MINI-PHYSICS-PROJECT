#ifndef TRACE_RECORDER_H
#define TRACE_RECORDER_H

#include <vector>
#include <string>
#include "Vector2D.h"
#include "SimulationState.h"
#include "World.h"
#include "Collision.h"

// TraceRecorder
// ----------------------------------------------------------------
// Bridges the C++ backend to the web frontend. It does NOT duplicate
// any physics logic - it simply records the World's state (via the
// existing SimulationState::captureFrom) after every step of a run,
// plus that step's collision events, and exports the whole sequence
// as a single JSON file.
//
// The web page never re-simulates anything: it only replays exactly
// what this C++ program computed. This keeps the actual OOP physics
// engine as the single source of truth, and the browser as a pure
// playback/telemetry viewer.
//
// OOP concepts demonstrated:
//   - Composition / reuse (built entirely on top of the existing
//     SimulationState class rather than re-deriving object data)
//   - STL (std::vector<SimulationState> frames,
//     std::vector<Collision> per-frame events)
//   - File I/O (a second, JSON-based file format, separate from
//     FileHandler's save/load text format - a deliberate, documented
//     example of supporting more than one file format for different
//     purposes)
// ----------------------------------------------------------------
class TraceRecorder {
private:
    struct FrameRecord {
        SimulationState state;
        std::vector<Collision> collisions;
    };

    std::vector<FrameRecord> frames;
    double timeStep;
    Vector2D boundaryMin;
    Vector2D boundaryMax;

public:
    TraceRecorder(double timeStep, const Vector2D& boundaryMin, const Vector2D& boundaryMax);

    void recordFrame(const World& world);
    void clear();
    size_t frameCount() const { return frames.size(); }

    // Writes the full recorded run to a JSON file the web visualizer can load.
    // Throws FileException if the file cannot be opened for writing.
    void exportToJSON(const std::string& filename) const;
};

#endif // TRACE_RECORDER_H
