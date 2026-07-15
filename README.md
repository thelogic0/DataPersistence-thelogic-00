# DataPersistence-thelogic-00

A C++20 console PoC proving JSON-file data persistence with full CRUD for three entities
(`Sample`, `Order`, `ProductionQueueItem`), verifying that data survives application restarts.

## Architecture

- **`JsonParser`** (`Json.h`/`Json.cpp`) — a hand-rolled JSON parser/serializer. No external JSON library is used.
- **Entities** (`Sample`, `Order`, `ProductionQueueItem`) — plain value classes with getters/setters, `toJson()`, and a static `fromJson(JsonParser&)`.
- **Repositories** (`SampleRepository`, `OrderRepository`, `ProductionQueueRepository`) — one per entity, each owning a single JSON file. Loads on construction, saves after every mutating call (`add`/`remove`/`update`), and rejects duplicate IDs on `add`.
- **`ConsoleApp`** — the interactive menu wiring all three repositories together.
- **`Main.cpp`** — Debug builds run the full gtest suite on launch; Release builds launch the interactive `ConsoleApp` menu.

## Build & Run

Open `DataPersistence-thelogic-00.slnx` in Visual Studio 2022+ and build, or from the command line:

```
MSBuild.exe DataPersistence-thelogic-00.slnx /p:Configuration=Debug /p:Platform=x64
MSBuild.exe DataPersistence-thelogic-00.slnx /p:Configuration=Release /p:Platform=x64
```

- **Debug**: running the executable runs all unit tests (currently 43 tests, all passing) and reports results to the console.
- **Release**: running the executable launches the interactive CRUD menu (Sample / Order / ProductionQueueItem).

## Data Files

`samples.json`, `orders.json`, and `production_queue.json` are created in the working directory the first time each entity is used, and are gitignored — they are runtime data, not source.
