# 1
Win API 기본 프레임 워크 구성
-> main.cpp
	-> System Class : Window 생성 및 Main Loop
		-> Input Class : 사용자 입출력
		-> DXEngine Class : Directx 사용

# 2
DirectX 초기화 관련 Class 추가
-> main.cpp
	-> System Class
		-> Input Class
		-> DXEngine Class
			-> DXD3D : DirectX 11 초기화

# 3
2차원상의 삼각형 그리기 / Model, Camera, Shader Class 추가
-> main.cpp
	-> System Class
		-> Input Class
		-> DXEngine Class
			-> DXD3D Class : DirectX 초기화 관련
			-> DXCamera Class : Camera 관련
			-> DXModel Class : Model 생성 ( Vertex, Index )
			-> DXShader Class : Shader 관련

# 4
2차원상의 삼각형 색깔 바꾸기 - 동적 리소스 사용 / MDParticle, DXPhysics 추가 예정
-> main.cpp
	-> System Class
		-> Input Class
		-> DXEngine Class
			-> DXD3D Class : DirectX 초기화 관련
			-> DXCamera Class : Camera 관련
			-> DXModel Class : Model 생성 ( Vertex, Index )
				-> MDParticle Class : 입자 관련 구조체
			-> DXShader Class : Shader 관련
			-> DXPhysics Class : 물리 연산 관련


# 5
간단한 조명 만들기 - 동적 리소스 사용 / MDParticle, DXPhysics 추가 예정
-> main.cpp
	-> System Class
		-> Input Class
		-> DXEngine Class
			-> DXD3D Class : DirectX 초기화 관련
			-> DXCamera Class : Camera 관련
			-> DXModel Class : Model 생성 ( Vertex, Index )
				-> MDParticle Class : 입자 관련 구조체
			-> DXShader Class : Shader 관련
				-> Light 적용할 수 있게 변경
			-> DXPhysics Class : 물리 연산 관련
			-> DXLight Class : Diffuse Light, Light Position 설정 관련


-> 조명 관련

 Normal Vector : 주어진 Object에서 나오는 방향
 Light Vector : 빛 벡터
 Light Intensity : 채도
 	-> Light Intensity = dot( Normal Vector * ( -1.0f ) , Light Vector )
 	-> 주어진 Object에 빛이 가해지는 정도 ( 0 ~ 1 )

 Diffuse Light : 분산광
 	-> 물체의 표면에서 분산되어 눈으로 바로 들어오는 빛
 	-> 각도에 따라 밝기가 다름
 		-> Light Intensity
 	-> Color = material Color * ( Light color * Light Intensity )
 Ambient Light : 주변광
  	-> 수많은 반사를 거쳐서 광원이 불분명한 빛
 	-> 물체를 덮고 있는 빛이며, 일정한 밝기와 색을 표현
 	-> Color = material Color * Ambient Light Color
 Specular Light : 반사광
 	-> 분산광과 달리 한방향으로 완전히 반사되는 빛
 	-> 반사되는 부분은 흰색의 광으로 보임
 Total Light : 실질적인 Object의 빛의 밝기


# 6
3차원 박스 만들기 / MDParticle, DXPhysics 추가 예정
-> main.cpp
	-> System Class
		-> Input Class
		-> DXEngine Class
			-> DXD3D Class : DirectX 초기화 관련
			-> DXCamera Class : Camera 관련
			-> DXModel Class : Model 생성 ( Vertex, Index )
				-> MDParticle Class : 입자 관련 구조체
			-> DXShader Class : Shader 관련
				-> Light 적용할 수 있게 변경
			-> DXPhysics Class : 물리 연산 관련
			-> DXLight Class : Diffuse Light, Light Position 설정 관련

-> Normal Vector
	-> final Normal Vector = World Matrix * Normal Vector
	-> World가 이동 혹은 회전함에 따라 최종 Normal Vector가 달라짐
	-> 이러한 Normal Vector는 Light로 표현되는 Color에도 영향을 끼침

# 7
목록
	-> 프레임워크 재정의
	-> 정반사광 추가
		-> 진짜 간단한 빛 구현
	-> 움직이는 3차원 박스 구현
	-> 간단한 글자 화면에 출력
	-> 사용자의 마우스 입력 관련 구현

구성
-> main.cpp
	-> SYSTEM
		-> DXENGINE
			-> DXD3D
			-> DXCAMERA
			-> DXMODEL
			-> DXLIGHT
			-> DXTEXT
			-> DXTEXTURE
		-> SUB
			-> SUBCPU
			-> SUBFPS
			-> SUBINPUT
			-> SUBLOG
			-> SUBTIMER

내용
-> main.cpp
	-> System
		- Window 초기화
		- MainLoop
		- DX, SUB 관리

		-> DXENGINE
			-> DXD3D
				- DirectX 초기화
				- Device, DeviceContext 관련
				- SwapChain, RenderTarget, DepthStencil, Blending 관련
				- World, Ortho, Projection Matrix & ViewPort 관련

			-> DXCAMERA
				- Camera 초기화
				- Camera Position, Rotation 관련
				- Camera View Matrix 관련

			-> DXMODEL
				- Model 초기화
					- Model Vertex, Index txt 파일 읽기 관련
						- Load Model
					- Image(texture) 관련
						- Load Texture
				- Vertex, Index 관련
					- Update, Init

			-> DXLIGHT
				- Light 초기화
					- Light 방향 및 각 성질들 설정
						- Set ... & Get ...
				-> DXL_SHADER
					- Light Shader 초기화
						- Light VS, PS hlsl 파일 읽기 관련
							- Init ...
					- Layout, SampleState, Buffer 관리
					- Light Render 관리

			-> DXTEXT
				-> DXT_FONT
					- Font 초기화
						- Font DDS(texture), txt 파일 읽기 관련
							- Load DDS & txt
					- 주어진 Text의 Vertex, Index 형성 관련
						- BuildVertexArray
				-> DXT_SHADER
					- Font Shader 초기화
						- Font VS, PS hlsl 파일 읽기 관련
							- Init ...
					- Layout, SampleState, Buffer 관리
					- Font Render 관리

			-> DXTEXTURE
				- Texture 파일 읽기
				- Texture 파일 내용 전해주기

		-> SUB
			-> SUBCPU - CPU 사용량 계산
			-> SUBFPS - FPS 계산
			-> SUBINPUT - User Input 관리
			-> SUBLOG - Log 출력 도우미
			-> SUBTIMER - Time 계산

# 8
--> DXTEXT에서 Sentence 생성 & Update 관련하여 밑의 내용 공부하자
	매개변수(parameter)와 인자(argument)
		-> 함수
			-> 인자(argument)가 매개변수(parameter)에 전달되는 형태로 동작

		ex)
		void func( int a, int b )
		{
			...
		}

		int main()
		{
			func( 1, 2);
			...
		}

		함수가 호출되게 되면,
			int x = 1;
			int y = 2;
		와 같이 인자가 매개변수에 전해짐

	인수 전달
		-> 값에 의한 전달
			-> 함수의 지역변수이나 매개변수로 인자의 값 복사
			-> 함수 호출이 끝나면 인자의 값 변화 X
			-> call by value 라고 부름

		-> 참조에 의한 전달
			-> 매개변수로 전해지는 인자의 메모리를 공유
				-> 매개변수와 인자의 주소 동일
			-> 함수 호출이 끝나면 인자의 값 변화 O
			-> C/C++은 return이 하나만 가능하기에, 이의 방법을 주로 사용
			-> 참조 방식
				-> 일반 & : L-value( 메모리 공간 O )
				-> const & : L-value, R-value( 메모리 공간 X )

		-> 주소에 의한 전달
			-> 값이 주소값으로 받는다고 생각하면 된다
			-> 간접 참조를 통하여 인자의 값 변경 가능
				-> 인자의 주소값을 매개변수가 지니고 있고
				-> 매개변수의 역참조를 통하여 인자로 접근하여 값 변경 가능

# 9
FPS, CPU 화면에 출력

구성
-> main.cpp
	-> SYSTEM
		-> DXENGINE
			-> DXD3D
			-> DXCAMERA
			-> DXMODEL
			-> DXLIGHT
			-> DXTEXT
			-> DXTEXTURE
		-> SUB
			-> SUBCPU
			-> SUBFPS
			-> SUBINPUT
			-> SUBLOG
			-> SUBTIMER

# 10
지면과의 충돌 구현
-> 3차원상의 강체 충돌 구현
	-> CCW(Counter Clockwise) 알고리즘
		-> Object
			- Normal Vector에 따라 Plane의 개수 설정
			- 총 Plane * Plane의 선분 개수
			- ex) 직육면체 2개 충돌 비교
				각각의 Plane의 개수 : 6개
				각 Plane의 선분의 개수 : 4개
				총 계산량 : 6*4*6*4 -> 매우 많아진다

			- 서로 다른 Object끼리 Normal Vector의 내적이 음수인 경우끼리 충돌 가능
				> 음수, 0, 양수 셋 다 계산하게 될 시, 계산량이 너무 많아짐
				> 충돌 시, Normal Vector를 정하는 기준은?

	-> mappedResource를 불러와서, Vertex와 Index에 memcpy를 시킨다.
		-> Output Buffer를 사용하여서 Input Buffer를 통해서 최종 생성된 Output Buffer를 얻는다
		ex memcpy( List, mappedResource.pData , sizeof( Type ) * Count )
		 동적 리소스를 진행했던 형식의 반대라고 생각하면 된다.
		 리소스에 대한 변환은 없다

	-> 강체의 이동
		-> Vertex의 변환
			-> Particle이 아닌 이상, 하나의 Object엔 수많은 Vertex와 Index가 존재
		-> World의 변환
			-> World의 변환을 사용해야
			-> 하나의 Model에 대한 다양한 물체들을 단순히 World를 추가함으로 구현할 수 있음

	-> 추후에 Model List 필요
		-> Model List에서 자신으로부터 Hit Box 범위 안에 있는 Model들만 충돌 테스트할 수 있도록 알고리즘 구현 필요

-> Vertex Shader의 Output Vertex 가져오
	-> D3D11_BIND_FLAG
		-> D3D11_BIND_VERTEX_BUFFER
			IA 단계에서 Binding
		-> D3D11_BIND_INDEX_BUFFER
			IA 단계에서 Binding
		-> D3D11_BIND_CONSTANT_BUFFER
			Shader 단계에서 Binding
		-> D3D11_BIND_SHADER_RESOURCE
			Shader 단계에서 Binding
		-> D3D11_BIND_STREAM_OUTPUT
			Stream Output 단계에서 출력 버퍼 바인딩
		-> D3D11_BIND_RENDER_TARGET
			Output Merge 단계에서 렌더링 대상 바인딩
		-> D3D11_BIND_DEPTH_STENCIL
			Output Merge 단계에서 깊이 스텐실 바인딩
		-> D3D11_BIND_UNORDERED_ACCESS
			순서가 지정되지 않은 액세스 리소스
		-> D3D11_BIND_DECORDER
			디코더 API
		-> D3D11_BIND_VIDEO_ENCODER
			비디오 API

# 11
2차원 카메라 이동 & Zoom In/Out
-> 카메라 이동 : 왼쪽 마우스 버튼 누른 상태에서 드래그
-> 줌 인&아웃 : Ctrl 키 누른 상태에서 휠 드래그
