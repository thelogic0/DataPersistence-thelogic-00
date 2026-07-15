# DataPersistence-thelogic-00

JSON 파일 기반 데이터 영속성을 검증하는 C++20 콘솔 PoC입니다. 시료(`Sample`), 주문(`Order`), 생산 큐 항목(`ProductionQueueItem`) 3개 엔티티에 대해 CRUD 전체를 구현하고, 프로그램을 재시작해도 데이터가 유지되는지 검증합니다.

## 아키텍처

- **`JsonParser`** (`Json.h`/`Json.cpp`) — 외부 JSON 라이브러리 없이 직접 구현한 JSON 파서/직렬화기.
- **엔티티** (`Sample`, `Order`, `ProductionQueueItem`) — getter/setter, `toJson()`, 정적 `fromJson(JsonParser&)`를 갖춘 순수 값 클래스.
- **Repository** (`SampleRepository`, `OrderRepository`, `ProductionQueueRepository`) — 엔티티별로 하나씩, 각각 JSON 파일 하나를 담당. 생성 시 파일을 로드하고, 변경 메서드(`add`/`remove`/`update`) 호출 후 즉시 저장하며, `add` 시 중복 ID를 거부.
- **`ConsoleApp`** — 세 Repository를 연결하는 대화형 메뉴.
- **`Main.cpp`** — Debug 빌드는 실행 시 gtest 전체 테스트를 수행하고, Release 빌드는 `ConsoleApp` 메뉴를 실행.

## 빌드 및 실행

`DataPersistence-thelogic-00.slnx`를 Visual Studio 2022 이상에서 열어 빌드하거나, 커맨드라인에서:

```
MSBuild.exe DataPersistence-thelogic-00.slnx /p:Configuration=Debug /p:Platform=x64
MSBuild.exe DataPersistence-thelogic-00.slnx /p:Configuration=Release /p:Platform=x64
```

- **Debug**: 실행 파일을 구동하면 전체 단위 테스트(현재 43개, 전부 통과)가 실행되고 결과가 콘솔에 출력됩니다.
- **Release**: 실행 파일을 구동하면 대화형 CRUD 메뉴(시료 / 주문 / 생산 큐 항목)가 시작됩니다.

## 데이터 파일

`samples.json`, `orders.json`, `production_queue.json`은 각 엔티티가 처음 사용될 때 작업 디렉터리에 생성되며, 소스가 아닌 런타임 데이터이므로 gitignore 처리되어 있습니다.
