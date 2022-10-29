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