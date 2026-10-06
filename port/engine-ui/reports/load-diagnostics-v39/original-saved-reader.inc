 bool saved(const std::string& name,bool& found,std::vector<std::uint8_t>& bytes,std::string& e){
  found=false;bytes.clear();
  if(name.empty()||name.find_first_of("/\\")!=std::string::npos||name.find('\0')!=std::string::npos){e="Invalid source LevelSavegame private filename";return false;}
  errno=0;auto* file=std::fopen((directory+"/"+name).c_str(),"rb");
  if(!file){if(errno==ENOENT){e.clear();return true;}e="Actual LevelSavegame file open failed: "+std::to_string(errno);return false;}
  found=true;std::array<std::uint8_t,8192> buffer{};bool okay=true;
  while(true){const auto count=std::fread(buffer.data(),1,buffer.size(),file);bytes.insert(bytes.end(),buffer.data(),buffer.data()+count);
   if(count<buffer.size()){okay=std::ferror(file)==0;break;}}
  if(std::fclose(file))okay=false;
  if(!okay){e="Actual LevelSavegame private file read/close failed";return false;}
  e.clear();return true;
 }
