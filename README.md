# SQP (Splatoon-inspired Gameplay Project)

## 1. 프로젝트 개요

SQP는 언리얼 엔진 5.6을 사용하여 개발한 스플래툰 스타일 3인칭 슈팅, 네트워크 페인팅 프로토타입 프로젝트입니다. 페인팅 메카닉을 중심으로 다양한 멀티플레이 콘텐츠를 제공합니다.

## 2. 핵심 기능

- **페인팅 시스템**: 월드에 페인트를 칠하고 관련 상호작용을 처리하는 스플래툰 스타일의 핵심 메카닉입니다. 언리얼 리슨 서버 기반의 네트워크 멀티플레이를 지원합니다.


- **다양한 게임플레이 모드**:
  - **Free Painting**: 페인트를 칠해 작품을 만드는데 집중하는 멀티플레이 모드입니다.
  - **Playground**: 페인트를 칠해 캐치마인드나 컴페티션 등의 경쟁을 지원하는 멀티플레이 모드입니다.

## 3. 프로젝트 구조

- **`Source/`**: C++ 소스 코드 디렉터리입니다.
  - `SQP/`: 메인 게임플레이 로직을 포함하는 런타임 모듈입니다.
    - `Default/`: 게임 플레이 전반에 사용되는 소스 코드 폴더입니다.
    - `Main/`: 메인 메뉴에 사용되는 소스 코드 폴더입니다.
    - `Lobby/`: 로비 메뉴에 사용되는 소스 코드 폴더입니다.
    - `PaintRoom/`: 페인트 룸에 사용되는 소스 코드 폴더입니다.
    - `PaintGaming/`: 컴페티션 등의 콘텐츠에 사용되는 소스 코드 폴더입니다.
    - `UI/`: UI에 사용되는 소스 코드 폴더입니다.
    - `Private/` 및 `Public/`: 기타 공용 소스 코드 폴더입니다.
    - `Yeomin/`: 윤여민 팀원이 사용하는 소스 코드 폴더입니다.
  - `GentleKit/`: 공용 유틸리티 또는 라이브러리 모듈입니다.
- **`Content/`**: 게임에 사용되는 모든 애셋 (블루프린트, 머티리얼, 맵 등)을 포함합니다.
  - `Splatoon/`: 페인팅 시스템의 핵심 애셋이 위치합니다.
- **`Config/`**: 프로젝트의 주요 설정 파일 (`DefaultEngine.ini`, `DefaultInput.ini` 등)이 위치합니다.

## 4. 기술 스택

- **엔진**: Unreal Engine 5.6
- **주요 언어**: C++
- **핵심 모듈 및 플러그인**:
  - `EnhancedInput`: 최신 입력 시스템
  - `StateTreeModule`, `GameplayStateTreeModule`: AI 로직 구현
  - `AIModule`: AI 시스템 기반
  - `UMG` `Slate` `SlateCore`: UI 구현
  - `Niagara`: 이펙트 구현
  - `NetCore` `OnlineSubsystem` `OnlineSubsystemUtils` `Sockets`: 멀티플레이
  - `ApplicationCore`, `Synthesis`: 시스템의 클립보드 접근
  - `HTTP` `ImageWrapper` `Json` `JsonUtilities`: AI 서버 통신
  - `Cbor`: 패키징에 필요함, 정체불명

## 5. 시작하기

### 사전 요구사항

- Unreal Engine 5.6
- Visual Studio 2022 또는 JetBrains Rider

### 설정 및 빌드

1.  저장소를 복제합니다.
2.  `SQP.uproject` 파일을 우클릭하여 **"Generate Visual Studio project files"**를 실행합니다.
3.  생성된 `SQP.sln` 파일을 열어 **Development Editor** 또는 **Development** 구성으로 빌드합니다.

## 6. 주요 레벨

에디터에서 다음 레벨을 열어 각 게임 모드를 테스트할 수 있습니다.

`Main`
`Lobby`
`CatchMind`
`ArtGallery`

에디터에서는 Main 레벨에서 StandAlone으로 2개의 PIE를 실행한 후에, 한쪽은 세션을 만들고 한쪽은 접속하는 방법으로 테스트합니다.
패키징에서는 2개의 프로세스에서 한쪽에서 세션을 만들고 한쪽은 접속하는 방법으로 테스트합니다. 만약 온라인 서브시스템으로 세션이 검색할 수
없다면 IP주소를 직접 입력해서 접속하는 방법도 가능합니다.

---

*본 프로젝트는 언리얼 엔진 공식 코딩 표준을 준수하는 것을 목표로 합니다.*
