# Combustion Simulation for Buildings
### **Contents**
[1. Overview](https://github.com/haram1117/FirePropagation/edit/main/README.md#-overview)

[2. Descriptions](https://github.com/haram1117/FirePropagation/edit/main/README.md#-descriptions)

[3. Results](https://github.com/haram1117/FirePropagation/edit/main/README.md#-results)

[4. What to Improve](https://github.com/haram1117/FirePropagation/edit/main/README.md#-what-to-improve)

[5. More Info](https://github.com/haram1117/FirePropagation/edit/main/README.md#-more-info)

## 📌 Overview
![UnrealEngine](https://img.shields.io/badge/unrealengine-black?style=for-the-badge&logo=unreal-engine&logoColor=white) <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white">

**Unreal Engine**을 이용하여 **건물의 연소 시뮬레이션**을 구현한 프로젝트입니다.


![image](https://user-images.githubusercontent.com/63827499/221190269-4be05c72-eba6-44fb-9be3-941597dde6a8.png)
- **경희 소용 X 펄어비스** (KHUSW X Pearl Abyss) 인재양성 프로그램 3차
- 프로젝트 기간 : **2022.10.20 ~ 2023.02.02**
- 프로젝트 인원 : **1인**

## 📕 Descriptions
### 프로젝트 설명
본 프로젝트는 **펄어비스(Pearl Abyss)와 경희대학교 소프트웨어융합학과**의 인재 양성 프로그램에서 진행한 프로젝트입니다.
게임에서 사용할 수 있는 기술을 **Unreal Engine** 상에서 구현하는 과제를 진행하였으며, 건물에서의 연소 시뮬레이션인 **Combustion Simulation for Buildings**를 주제로 진행하였습니다.

### 타겟 논문 및 관련 연구
#### 🔎 [Interactive Wood Combustion for Botanical Tree Models](https://repository.kaust.edu.sa/bitstream/10754/626814/1/a197-pirk.pdf)
![image](https://user-images.githubusercontent.com/63827499/221209687-dfca2f94-3f5d-47cc-b9b9-f807bd085546.png)

해당 논문을 이용하여 나무에서의 연소 시뮬레이션이 건물에서 동작할 수 있도록 **구조 및 알고리즘을 수정**하고, **Vertex 기반의 Heat Transfer에 대한 아이디어를 구축**하였습니다.

#### 🔎 [Heat Release and Mass Loss Rate Measurements for Selected Materials](https://nvlpubs.nist.gov/nistpubs/Legacy/IR/nbsir84-2960.pdf)
![image](https://user-images.githubusercontent.com/63827499/221211407-89a870db-6bf7-40a2-b3cf-28b5aaa5c613.png)

위 연구를 통해 각 건물의 재질에 따라 Simulation에 영향을 미치도록 각종 **Physical & Biological Attributes**을 조사하였습니다.

### 프로젝트 특징
#### 1. Mesh의 Vertex 기반 Heat Transfer
![image](https://user-images.githubusercontent.com/63827499/221214638-687f2300-b4a8-4779-a1e3-093eb3f59e0e.png)

건물에서의 열 전달을 표현하기 위해 Vertex를 기준으로 Heat Transfer Process를 수행하였습니다.
- 💡 Idea
	- **어떻게 하면 건물에서의 열전달을 사실적으로 표현할 수 있을지**에 대해 고민했습니다.
	- 건물을 Voxel 단위, Block 단위등으로 나누거나 Procedural Building 생성을 통한 Vertex 규칙을 생성하는 등의 아이디어 끝에, **"Vertex를 기준으로 열전달을 표현하면 어떨까?"** 라는 생각을 하게 되었습니다.
	- 위 그림과 같이 각 Vertex에서 Adjacent Vertex를 따라 Heat Transfer가 이루어지는 **알고리즘을 구축**하게 되었습니다.
	-  열전달을 시각화하기 위해서는 **Unreal Engine의 Mesh Description, Vertex Color**를 사용하였습니다.
- 🎈 Result
	- 열전달이 **Vertex Color로 인해 시각화**되기 때문에 건물에서의 불이 어떤 식으로 퍼질지에 대한 예상이 가능합니다.
	- Vertex를 기준으로 Heat Transfer가 이루어지기 때문에 **건물 뿐만 아니라 다양한 3D Model**에 대해서도 Combustion Simulation을 적용할 수 있습니다.

## 📋 Results
### 프로젝트 결과물
#### 1. Mesh의 Vertex 기반 Heat Transfer
![image](https://user-images.githubusercontent.com/63827499/221218236-612ce434-3769-4614-a8f8-dc1505fa0562.png)

![image](https://user-images.githubusercontent.com/63827499/221218333-580b10d3-6ca3-4a17-a57a-8e8ae07452b7.png)

#### 2. Physical & Biological Attributes
![image](https://user-images.githubusercontent.com/63827499/221218601-05ca2714-72eb-472c-ba30-82e381d7fd36.png)

## 🔨 What to Improve
- Vertex 단에서 Heat Transfer가 이루어지기 때문에 high polygon의 model들에게는 적합하지 않아 최적화가 필요합니다.
- 사용자가 직접 attribute들을 조작하여야 하기에 건물의 재질에 따른 몇 가지의 preset이 제공되어야 합니다.
- 주변 Air Temperature에도 영향을 받는다면 더 실감나는 시뮬레이션 구현이 가능할 것입니다.
- 불과 연기가 수직거리로 빠르게 퍼진다는 점을 반영하여 Heat Transfer의 각 방향으로의 가중치를 부여하여 조금 더 사실적인 표현이 필요합니다.

## 📢 More Info
#### 📧 haram1117@khu.ac.kr 
- 프로젝트 회고록
	- https://dev-haram.online/66
- Showcase Video
	- https://www.youtube.com/watch?v=7byZo9cqBuI

