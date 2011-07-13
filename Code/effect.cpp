#include "effect.h"
#include <D3DX11.h>

//a

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

Effect::Effect() : input_layout_(NULL) {

}
Effect::~Effect() {

}

int Effect::Deinitialize() {
  SafeRelease(&input_layout_);
  if (context_) {
    context_->DestroyPixelShader(pixel_shader_);
    context_->DestroyVertexShader(vertex_shader_);
  }
  return S_OK;
}

int Effect::CreateFromMemory(void* data,uint32_t size) {

  graphics::ShaderBlob vs_blob;
  HRESULT hr = context_->CompileShaderFromMemory(data,size,"VS", "vs_4_0", vs_blob);
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
  hr = context_->CompileShaderFromMemory(data,size,"PS", "ps_4_0",ps_blob);
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

  D3D11_INPUT_ELEMENT_DESC *input_layout_desc;
  UINT element_count = 0;
  {
    ID3D11ShaderReflection* pReflector = NULL;

    hr = D3DReflect( vs_blob.data(),vs_blob.size(), IID_ID3D11ShaderReflection, (void**) &pReflector);
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

  /*    // Define the input layout
  D3D11_INPUT_ELEMENT_DESC layout[] =
  {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };
  UINT numElements = ARRAYSIZE( layout );
  */

  ContextD3D11* c11 = (ContextD3D11*)context_;

  // Create the input layout
  hr = c11->device()->CreateInputLayout(input_layout_desc,element_count,vs_blob.data(),vs_blob.size(), &input_layout_);
  for (UINT i=0;i<element_count;++i) {
    delete []input_layout_desc[i].SemanticName;
  }
  delete []input_layout_desc;
  if( FAILED( hr ) )
    return hr;

  return S_OK;
}

int Effect::Begin() {
  ContextD3D11* c11 = (ContextD3D11*)context_;
  // Set the input layout
  c11->device_context()->IASetInputLayout( input_layout_ );
  context_->SetShader(vertex_shader_);
  context_->SetShader(pixel_shader_);
  //c11->SetShader(geometry_shader_);
  return S_OK;
}

}
