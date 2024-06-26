#include "PreCompile.h"
#include "EngineCore.h"
#include "EngineGraphicDevice.h"
#include "EngineVertex.h"
#include "EngineMesh.h"
#include "EngineTexture.h"
#include "EngineVertexShader.h"
#include "EnginePixelShader.h"
#include "EngineRasterizer.h"
#include "EngineMaterial.h"

void UEngineGraphicDevice::EngineResourcesRelease()
{
	// 어차피 자동으로 지워지는 리소스들을 왜 굳이 여기서 클리어를 직접 해주지?
	// 엔진이 종료되는 시점에 텍스처를 모두다 삭제한다.
	UEngineSound::ResourcesRelease();
	UEngineTexture::ResourcesRelease();

	// Mesh
	UEngineVertexBuffer::ResourcesRelease();
	UEngineIndexBuffer::ResourcesRelease();
	UEngineMesh::ResourcesRelease();

	// 머티리얼
	UEngineVertexShader::ResourcesRelease();
	UEnginePixelShader::ResourcesRelease();
	UEngineRasterizer::ResourcesRelease();
	UEngineMaterial::ResourcesRelease();
}

// 인풋어셈블러 1과 인풋어셈블러 2의 리소스들을 만들어내는 이니셜라이즈
void MeshInit()
{
	FEngineVertex::Info.AddInputLayOut("POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	FEngineVertex::Info.AddInputLayOut("TEXCOORD", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);

	{
		std::vector<FEngineVertex> VertexData;
		VertexData.resize(4);

		{
			// 00   10
			// 0    1
			// 01   11
			// 3    2

			VertexData[0] = { {-0.5f, 0.5f, 0.0f, 1.0f} , {0.0f, 0.0f} };
			VertexData[1] = { {0.5f, 0.5f, 0.0f, 1.0f} , {2.0f, 0.0f} };
			VertexData[2] = { {0.5f, -0.5f, 0.0f, 1.0f}, {2.0f, 2.0f} };
			VertexData[3] = { {-0.5f, -0.5f, 0.0f, 1.0f}, {0.0f, 2.0f} };
			std::shared_ptr<UEngineVertexBuffer> VertexBuffer = UEngineVertexBuffer::Create("Rect", VertexData);
		}

		// 삼각형을 어떻게 그릴지에 대한 순서.
		std::vector<UINT> IndexData = { 0, 1, 2, 0, 2, 3 };

		std::shared_ptr<UEngineIndexBuffer> IndexBuffer = UEngineIndexBuffer::Create("Rect", IndexData);

		UEngineMesh::Create("Rect");

		{
			VertexData[0].POSITION *= 2.0f;
			VertexData[1].POSITION *= 2.0f;
			VertexData[2].POSITION *= 2.0f;
			VertexData[3].POSITION *= 2.0f;
			std::shared_ptr<UEngineVertexBuffer> VertexBuffer = UEngineVertexBuffer::Create("FullRect", VertexData);
		}

		UEngineMesh::Create("FullRect", "FullRect", "Rect");
	}

	// 인풋레이아웃 정보
	// GEngine->GetDirectXDevice()->CreateInputLayout()


}

// 픽셀 및 버텍스쉐이더를 이니셜라이즈
void ShaderInit()
{
	UEngineDirectory Dir;
	Dir.MoveToSearchChild("EngineShader");

	std::vector<UEngineFile> Files = Dir.GetAllFile({ ".fx", "hlsl" });

	for (size_t i = 0; i < Files.size(); i++)
	{
		std::string FullPath = Files[i].GetFullPath();
		std::string AllShaderCode = Files[i].GetString();

		{
			// 앞에서부터 뒤로
			size_t ShaderEntryEnd = AllShaderCode.find("_VS("/*, 0*/);

			if (std::string::npos != ShaderEntryEnd)
			{
				// 뒤에서부터 앞으로
				size_t ShaderEntryStart = AllShaderCode.rfind(" ", ShaderEntryEnd);
				std::string EntryName = AllShaderCode.substr(ShaderEntryStart + 1, ShaderEntryEnd - ShaderEntryStart - 1);
				EntryName += "_VS";

				UEngineVertexShader::Load(FullPath.c_str(), EntryName);
			}
		}

		{
			// 앞에서부터 뒤로
			size_t ShaderEntryEnd = AllShaderCode.find("_PS("/*, 0*/);

			if (std::string::npos != ShaderEntryEnd)
			{
				// 뒤에서부터 앞으로
				size_t ShaderEntryStart = AllShaderCode.rfind(" ", ShaderEntryEnd);
				std::string EntryName = AllShaderCode.substr(ShaderEntryStart + 1, ShaderEntryEnd - ShaderEntryStart - 1);
				EntryName += "_PS";

				UEnginePixelShader::Load(FullPath.c_str(), EntryName);
			}
		}
	}

	// UEngineVertexShader::Load("D:ENgineShader\MeshVertexShader", "AAAA_VS");

	//UEngineVertexShader::Load("AAA.png", EntryName);
	//UEngineVertexShader::Load("BBB.png", EntryName);
}

void SettingInit()
{
	//D3D11_FILL_MODE FillMode;
	//D3D11_CULL_MODE CullMode;
	//BOOL FrontCounterClockwise;
	//INT DepthBias;
	//FLOAT DepthBiasClamp;
	//FLOAT SlopeScaledDepthBias;
	//BOOL DepthClipEnable;
	//BOOL ScissorEnable;
	//BOOL MultisampleEnable;
	//BOOL AntialiasedLineEnable;

	{
		D3D11_RASTERIZER_DESC Desc = {};

		// 면으로 그려라
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		// 선으로 그려라.
		// Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

		// 앞면이건 뒷면이건 다 그려라.
		// 우리 외적으로 앞
		// 앞면 그리지마

		// Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		// 뒷면 그리지마
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		// 앞면과 뒷면
		// 시계방향이면 뒷면으로 봅니다.

		Desc.AntialiasedLineEnable = TRUE;
		Desc.DepthClipEnable = TRUE;

		// 레스터라이저 세팅
		UEngineRasterizer::Create("EngineBasic", Desc);
	}

	{
		D3D11_SAMPLER_DESC Desc = {};

		// 옵션바꾸면서 설명드리겠습니다.
		// 가로
		Desc.AddressW = Desc.AddressV = Desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

		// MIP MAP
		// MIN_MAG_MIP 나보다 클때든 작을때는 
		// MIP 나보다 작을때
		// MAG 나보다 클때
		// MIN 확대할때
		// 언제나 POINT로 샘플링히라
		// POINT 보간을 하지 않고 색깔을 추출해라.
		// Liner 는 보간을 하고 추출해라.
		Desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;

		// 밉맵의 개념에 대해서 이해해야한다.
		Desc.MipLODBias = 0.0f; // 보간하지 않는다.
		Desc.MaxAnisotropy = 1;
		// 특정 값의 비교를 통해서 색깔을 출력해야할때 여러곳에서 사용되는 옵션인데.
		// 이건 그냥 무조건 비교하지 말고 출력해라.
		Desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;

		Desc.MinLOD = -FLT_MAX;
		Desc.MaxLOD = FLT_MAX;

		// 필터

		UEngineSampler::Create("POINT", Desc);
	}


}

void MaterialInit()
{


	std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("2DImage");
	Mat->SetPixelShader("ImageShader.fx");
	Mat->SetVertexShader("ImageShader.fx");
	Mat->SetRasterizer("EngineBasic");

}


// 엔진에서 박스하나도 지원안해주면 
void UEngineGraphicDevice::EngineResourcesInit()
{
	MeshInit();
	ShaderInit();
	SettingInit();
	MaterialInit();
}