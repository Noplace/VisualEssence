/*****************************************************************************************************************
* Copyright (c) 2012 Khalid Ali Al-Kooheji                                                                       *
*                                                                                                                *
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and              *
* associated documentation files (the "Software"), to deal in the Software without restriction, including        *
* without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        *
* copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the       *
* following conditions:                                                                                          *
*                                                                                                                *
* The above copyright notice and this permission notice shall be included in all copies or substantial           *
* portions of the Software.                                                                                      *
*                                                                                                                *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT          *
* LIMITED TO THE WARRANTIES OF MERCHANTABILITY, * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.          *
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, * DAMAGES OR OTHER LIABILITY,      *
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE            *
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                                         *
*****************************************************************************************************************/
#pragma once



namespace ve {

struct FileData {
  const char* filename;
  size_t length;
  void* data;
};


inline std::string GetExeFileName()
{
  char buffer[MAX_PATH];
  GetModuleFileName( NULL, buffer, MAX_PATH );
  return std::string(buffer);
}

inline std::string GetExePath() 
{
  std::string f = GetExeFileName();
  return f.substr(0, f.find_last_of( "\\/" ));
}

inline concurrency::task<FileData> ReadDataAsync(const char* filename) {

    return concurrency::create_task([filename](){
        //*data_pointer = nullptr;
        //data_length = 0;
        
        FILE* fp = nullptr;
        fopen_s(&fp,filename,"rb");
	      if (fp==nullptr)
		      return FileData { 0,0,0 };
        fseek(fp,0,SEEK_END);
      //#ifdef _WIN64
        //data_length = _ftelli64(fp);
      //#else
        auto data_length = ftell(fp);
      //#endif
        fseek(fp,0,SEEK_SET);
        uint8_t* data = new uint8_t[data_length];
        fread(data,1,data_length,fp);
        fclose(fp);

        FileData fd;
        fd.data = data;
        fd.length = data_length;
        fd.filename = filename;
        return fd; 
    });

  /*return std::async([&filename](){
    
    //*data_pointer = nullptr;
    //data_length = 0;
    FILE* fp = nullptr;
    fopen_s(&fp,filename,"rb");
	  if (fp==nullptr)
		  return FileData { 0,0,0 };
    fseek(fp,0,SEEK_END);
  //#ifdef _WIN64
    //data_length = _ftelli64(fp);
  //#else
    auto data_length = ftell(fp);
  //#endif
    fseek(fp,0,SEEK_SET);
    uint8_t* data = new uint8_t[data_length];
    fread(data,1,data_length,fp);
    fclose(fp);

    FileData fd;
    fd.data = data;
    fd.length = data_length;
    fd.filename = filename;

    return fd;
  });*/
}

/*
inline Concurrency::task<Platform::Array<byte>^> ReadDataAsync(Platform::String^ filename)
	{
		using namespace Windows::Storage;
		using namespace Concurrency;
		
		auto folder = Windows::ApplicationModel::Package::Current->InstalledLocation;
		
		return create_task(folder->GetFileAsync(filename)).then([] (StorageFile^ file) 
		{
			return FileIO::ReadBufferAsync(file);
		}).then([] (Streams::IBuffer^ fileBuffer) -> Platform::Array<byte>^ 
		{
			auto fileData = ref new Platform::Array<byte>(fileBuffer->Length);
			Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(fileData);
			return fileData;
		});
	}
*/

}