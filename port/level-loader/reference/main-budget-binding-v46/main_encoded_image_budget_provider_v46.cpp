#include "main_encoded_image_budget_provider_v46.hpp"
#include <zip_asset_pack_v1.hpp>
namespace dh2::loader {
struct MainEncodedImageBudgetProviderV46::Ticket {
 resources::CpuAdmissionV40 admission;
 std::shared_ptr<void> filesystem_owner;
 std::string uri;
 std::uint32_t bytes{};
};
bool MainEncodedImageBudgetProviderV46::admit_before_read(const MaterialImageIdentityV43& id,
 const std::string& uri,std::shared_ptr<void>& out,std::string& e){
 auto renderer=input_.actual_renderer_owner.lock();
 if(!renderer||!input_.filesystem_owner||!input_.actual_main_budget||!input_.original_read_admitted){e="Required live SAME main renderer/cache/budget admission producer";return false;}
 if(pending_.lock()){e="Main encoded image admission reentered before pending read";return false;}
 if(id.state!=MaterialImageStateV43::authored_image){e="Source no-image does not request encoded admission";return false;}
 std::string canonical;if(!assets::ZipAssetPackV1::key(uri,canonical,e))return false;if(canonical!=uri){e="Required canonical full URI before main admission";return false;}
 // Current OriginalCache API exposes metadata in its before_payload hook,
 // after local entry verification and BEFORE constructing the byte vector.
 // Query that actual verified size with a deliberate internal no-payload stop.
 bool metadata=false,found=false;std::uint32_t size=0;std::vector<std::uint8_t> probe;
 const bool complete=input_.original_read_admitted(uri,found,probe,[&](std::uint32_t n,std::string& e){metadata=true;size=n;e="V46 metadata-only stop before payload allocation";return false;},e);
 if(!metadata){if(complete&&!found)e="Required exact original image URI missing before admission: "+uri;return false;}
 if(complete||!probe.empty()||probe.capacity()!=0){e="Main metadata probe unexpectedly allocated/delivered image bytes";return false;}
 if(!size){e="Original image has empty encoded allocation";return false;}
 auto ticket=std::make_shared<Ticket>();ticket->filesystem_owner=input_.filesystem_owner;ticket->uri=uri;ticket->bytes=size;
 if(!ticket->admission.reserve(input_.actual_main_budget,input_.scope,size,e))return false;
 pending_=ticket;out=std::move(ticket);e.clear();return true;
}
bool MainEncodedImageBudgetProviderV46::read(const std::string& uri,bool& found,
 std::vector<std::uint8_t>& out,std::string& e){
 auto renderer=input_.actual_renderer_owner.lock();auto ticket=pending_.lock();
 if(!renderer||!ticket||ticket->uri!=uri){e="Required SAME live renderer/preadmitted encoded image ticket";return false;}
 pending_.reset();bool delivered=false;
 if(!input_.original_read_admitted(uri,delivered,out,[&](std::uint32_t bytes,std::string& e){if(bytes!=ticket->bytes){e="Original image allocation metadata changed after admission";return false;}return true;},e))return false;
 if(!delivered){found=false;e.clear();return true;}
 if(out.size()!=ticket->bytes){e="Original image delivered allocation differs from main admission";return false;}
 if(!ticket->admission.commit(e))return false;found=true;e.clear();return true;
}
bool MainEncodedImageBudgetProviderV46::image_services(
 const std::function<bool(const MaterialImageIdentityV43&,std::string&,std::string&)>& resolver,
 MaterialImageServicesV43& out,std::string& e){
 auto self=weak_from_this().lock();if(!self||!resolver||!input_.filesystem_owner||!input_.actual_main_budget||!input_.original_read_admitted){e="Required shared actual main encoded image provider/resolver";return false;}
 out={self,resolver,[self](const std::string& uri,bool& found,std::vector<std::uint8_t>& bytes,std::string& e){return self->read(uri,found,bytes,e);}};e.clear();return true;
}
}
