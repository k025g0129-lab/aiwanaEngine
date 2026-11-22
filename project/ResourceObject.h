#pragma once
#include<Windows.h>
#include<cstdint>
#include<string>
#include<format>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include "DebugLog.h"
#include "DebugLogMacro.h"
#include <dbghelp.h>
#include<strsafe.h>
#include<dxgidebug.h>
#include<dxcapi.h>
#include"function.h"
#include"externals/DirectXTex/DirectXTex.h"

class ResourceObject{
public:


	ResourceObject(ID3D12Resource* resource)
		:resource_(resource)
	{}


	~ResourceObject() {
		if (resource_){
			resource_->Release();
		}
	
	
	};

	ID3D12Resource* Get() { return resource_; }

private:
	ID3D12Resource* resource_;


};

