#include "../user_properties_v2.hpp"
#include <cassert>
#include <iostream>
int main(){std::map<std::string,std::string> actual,expected;std::string error;unsigned n=0;
expected={{"floortypes","door"}};
assert(dh2::world::user_properties_v2("floortypes = %22door%22\r\n",actual,error));assert(actual==expected);++n;
expected={{"floortypes","hole"}};
assert(dh2::world::user_properties_v2("floortypes = %22hole%22",actual,error));assert(actual==expected);++n;
expected={{"floortypes","water"}};
assert(dh2::world::user_properties_v2("floortypes = %22water%22",actual,error));assert(actual==expected);++n;
expected={{"floortypes","wood"}};
assert(dh2::world::user_properties_v2("floortypes = %22wood%22",actual,error));assert(actual==expected);++n;
expected={};
assert(dh2::world::user_properties_v2("",actual,error));assert(actual==expected);++n;
expected={{"key"," value"}};
assert(dh2::world::user_properties_v2(" key = value",actual,error));assert(actual==expected);++n;
expected={{"a","two"}};
assert(dh2::world::user_properties_v2("a=one\na=two",actual,error));assert(actual==expected);++n;
expected={{"foo","water"}};
assert(dh2::world::user_properties_v2("foo_bar=%22water%22",actual,error));assert(actual==expected);++n;
expected={{"x","hole wall"}};
assert(dh2::world::user_properties_v2("x=pre%22hole wall%22post",actual,error));assert(actual==expected);++n;
expected={{"a","%22unclosed"}};
assert(dh2::world::user_properties_v2("a=%22unclosed",actual,error));assert(actual==expected);++n;
expected={{"noequal",""}};
assert(dh2::world::user_properties_v2("noequal",actual,error));assert(actual==expected);++n;
expected={};
assert(dh2::world::user_properties_v2("   = value",actual,error));assert(actual==expected);++n;
expected={{"99key","void"}};
assert(dh2::world::user_properties_v2("99key=%22void%22",actual,error));assert(actual==expected);++n;
expected={{"a",""},{"b",""}};
assert(dh2::world::user_properties_v2("a=\nb=%22%22",actual,error));assert(actual==expected);++n;
expected={{"a","one"}};
assert(dh2::world::user_properties_v2("a=%22one%22%22two%22",actual,error));assert(actual==expected);++n;
assert(!dh2::world::user_properties_v2(nullptr,actual,error));std::cout<<"PASS original UserProperties golden cases "<<n<<" plus null input\n";}
