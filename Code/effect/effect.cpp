#include "effect.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}

inline DXGI_FORMAT GetFormatFromShader(const D3D11_SIGNATURE_PARAMETER_DESC& input) {
  DXGI_FORMAT result = DXGI_FORMAT_UNKNOWN;

  if (input.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
    if (input.Mask==0x1)
      return DXGI_FORMAT_R32_UINT;
    if (input.Mask==0x3)
      return DXGI_FORMAT_R32G32_UINT;
    if (input.Mask==0x7)
      return DXGI_FORMAT_R32G32B32_UINT;
    if (input.Mask==0xf)
      return DXGI_FORMAT_R32G32B32A32_UINT;
  }

  if (input.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
    if (input.Mask==0x1)
      return DXGI_FORMAT_R32_SINT;
    if (input.Mask==0x3)
      return DXGI_FORMAT_R32G32_SINT;
    if (input.Mask==0x7)
      return DXGI_FORMAT_R32G32B32_SINT;
    if (input.Mask==0xf)
      return DXGI_FORMAT_R32G32B32A32_SINT;
  }


  if (input.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
    if (input.Mask==0x1)
      return DXGI_FORMAT_R32_FLOAT;
    if (input.Mask==0x3)
      return DXGI_FORMAT_R32G32_FLOAT;
    if (input.Mask==0x7)
      return DXGI_FORMAT_R32G32B32_FLOAT;
    if (input.Mask==0xf)
      return DXGI_FORMAT_R32G32B32A32_FLOAT;
  }

  return result;
}

inline UINT CalculateShaderElementSize(const D3D11_SIGNATURE_PARAMETER_DESC& input) {
  if (input.Mask==0x1)
    return 4;
  if (input.Mask==0x3)
    return 8;
  if (input.Mask==0x7)
    return 12;
  if (input.Mask==0xf)
    return 16;

  return 0;
}

namespace graphics {

Effect::Effect() {

}
Effect::~Effect() {

}

int Effect::Deinitialize() {

  if (context_) {
    context_->DestoryInputLayout(input_layout_);
    context_->DestroyShader(pixel_shader_);
    context_->DestroyShader(vertex_shader_);

    if (vs_list_) {
      for (int i=0;i<vs_count;++i) {
        context_->DestroyShader(vs_list_[i]);
      }
    }

    if (ps_list_) {
      for (int i=0;i<ps_count;++i) {
        context_->DestroyShader(ps_list_[i]);
      }
    }

    if (gs_list_) {
      for (int i=0;i<gs_count;++i) {
        context_->DestroyShader(gs_list_[i]);
      }
    }

  }
  return S_OK;
}

int Effect::CreateInputLayoutFromShader(ShaderBlob& vs_blob) {
  
  D3D11_INPUT_ELEMENT_DESC *input_layout_desc;
  UINT element_count = 0;
  {
    ID3D11ShaderReflection* pReflector = NULL;

    int hr = D3DReflect( vs_blob.data(),vs_blob.size(), IID_ID3D11ShaderReflection, (void**) &pReflector);
    D3D11_SHADER_DESC desc;
    pReflector->GetDesc( &desc );
    input_layout_desc = new D3D11_INPUT_ELEMENT_DESC[desc.InputParameters];
    
    UINT offset=0;
    for ( UINT i = 0; i < desc.InputParameters; i++ )  {
      D3D11_SIGNATURE_PARAMETER_DESC input_desc;
      char tempName[32];
      pReflector->GetInputParameterDesc( i, &input_desc );
      strcpy_s(tempName,input_desc.SemanticName);
      input_layout_desc[i].SemanticIndex = input_desc.SemanticIndex;
      input_layout_desc[i].InputSlot = input_desc.Stream;
      input_layout_desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
      input_layout_desc[i].InstanceDataStepRate = 0;
      input_layout_desc[i].AlignedByteOffset = offset;
      input_layout_desc[i].Format = GetFormatFromShader(input_desc);//.ComponentType
      input_layout_desc[i].SemanticName = new char[32];
      //strcpy_s((char (&)[32])input_layout_desc[i].SemanticName,tempName);
      strcpy((char*)input_layout_desc[i].SemanticName,tempName);
      offset += CalculateShaderElementSize(input_desc);
      ++element_count;
      // Do something with the description…
    }

  }

  ContextD3D11* c11 = (ContextD3D11*)context_;
  ID3D11InputLayout* il_pointer;
  //Create the input layout
  int hr = c11->device()->CreateInputLayout(input_layout_desc,element_count,vs_blob.data(),vs_blob.size(), &il_pointer);
  for (UINT i=0;i<element_count;++i) {
    delete []input_layout_desc[i].SemanticName;
  }
  delete []input_layout_desc;
  if( FAILED( hr ) )
    return hr;

  input_layout_.set_pointer(il_pointer);

  return S_OK;
}

int Effect::CreateFromMemory(void* data,uint32_t size) {

  graphics::ShaderBlob vs_blob;
  HRESULT hr = context_->CompileShaderFromMemory(data,size,"VS", "vs_5_0", vs_blob);
  if( FAILED( hr ) )
  {
      MessageBox( NULL,
                  "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK );
      return hr;
  }

  // Create the vertex shader
  hr = context_->CreateVertexShader(vs_blob.data(),vs_blob.size(),vertex_shader_);
  if( FAILED( hr ) ) {    
        return hr;
  }

  // Compile the pixel shader
  graphics::ShaderBlob ps_blob;
  //hr = CompileShaderFromFile( L"Content\\Shaders\\main.fx", "PS", "ps_4_0", &pPSBlob );
  hr = context_->CompileShaderFromMemory(data,size,"PS", "ps_5_0",ps_blob);
  if( FAILED( hr ) ) {
      MessageBox( NULL,
                  "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK );
      return hr;
  }

  // Create the pixel shader
  hr = context_->CreatePixelShader( ps_blob.data(), ps_blob.size(), pixel_shader_ );
  if( FAILED( hr ) )
      return hr;
    

  // Compile the geometry shader
  graphics::ShaderBlob gs_blob;
  hr = context_->CompileShaderFromMemory(data,size,"GS", "gs_4_0",gs_blob);
  if( !FAILED( hr ) ) {
      // Create the pixel shader
    hr = context_->CreateGeometryShader( gs_blob.data(), gs_blob.size(), geometry_shader_ );
  }

  return CreateInputLayoutFromShader(vs_blob);
}

int Effect::CreateFromMemory2(Effect::Entry* entry_list, void* data, uint32_t size) {
  int hr = S_OK;

  ps_count = 0;
  vs_count = 0;
  gs_count = 0;
  for (Entry* e = entry_list; e->name != NULL; ++e) {

    if (e->version[0] == 'p' || e->version[0] == 'P') {
      ++ps_count;
    } else if(e->version[0] == 'v' || e->version[0] == 'v') {
      ++vs_count;
    } else if(e->version[0] == 'g' || e->version[0] == 'g') {
      ++gs_count;
    }
  }

  vs_list_ = new VertexShader[vs_count];
  gs_list_ = new GeometryShader[gs_count];
  ps_list_ = new PixelShader[ps_count];

  int vs_index = 0,ps_index = 0,gs_index = 0;
  for (Entry* e = entry_list; e->name != NULL; ++e) {
    graphics::ShaderBlob blob;
    hr = context_->CompileShaderFromMemory(data,size,e->name, e->version,blob);
    if( FAILED( hr ) ) {
       return hr;
    }


    if (e->version[0] == 'p' || e->version[0] == 'P') {
      hr = context_->CreatePixelShader( blob.data(), blob.size(), ps_list_[ps_index++]  );
    } else if(e->version[0] == 'v' || e->version[0] == 'v') {
       hr = context_->CreateVertexShader( blob.data(), blob.size(), vs_list_[vs_index] );
       if (vs_index == 0) {
         CreateInputLayoutFromShader(blob);
       }
       ++vs_index;
    } else if(e->version[0] == 'g' || e->version[0] == 'g') {
       hr = context_->CreateGeometryShader( blob.data(), blob.size(), gs_list_[gs_index++]  );
    } else if(e->version[0] == 'h' || e->version[0] == 'h') {
       
    }

    if( FAILED( hr ) )
        return hr;

  }

  return hr;
}

int Effect::Begin() {
  ContextD3D11* c11 = (ContextD3D11*)context_;
  // Set the input layout
  context_->SetInputLayout(input_layout_);
  context_->SetShader(vertex_shader_);
  context_->SetShader(pixel_shader_);
  //if (geometry_shader_.internal_pointer() != NULL)
  context_->SetShader(geometry_shader_);
  return S_OK;
}

}
