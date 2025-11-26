## Traffic Simulation Engine

This project is a **work in progress** traffic simulation engine.  
The goal is to build a small engine that can model vehicles moving through a customisable road network, and visualise the simulation.

### Goals
- **Core simulation**: represent vehicles, roads, and simple traffic rules, and step the simulation forward in time.
- **Rendering / UI**: visualise the traffic and provide basic controls (start, pause, reset).
- **Experimentation**: make it easy to tweak parameters (vehicle count, speeds, timing) and see how the system behaves.

### Tech stack & learning goals
- **Tech**: C++, CMake, Qt, OpenGL (for UI/rendering).
- **Learning focus**: practising C++, exploring basic simulation design, and improving understanding of graphics/OpenGL, UI, and code architecture.

### Building & running
This project uses **CMake**. A typical build flow looks like:

```bash
mkdir build
cd build
cmake ..
make
./TrafficSimApp
```

