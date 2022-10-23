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


# 4
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