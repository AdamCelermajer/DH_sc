#pragma once
#include "application_services_owner_v5.hpp"
#include <string>
#include <memory>
namespace model_renderer {
//Enroll at actual Application C1/native initialization, before gameplay.
//Application C1 constructs distinct job1(argument2) and job2(argument3).
//Repeated enrollment only borrows these SAME original process owners.
bool enroll_process_settings_job_v102(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
//Distinct Start2(317e98)->threadfun3(317f90)->saveSettings(46cb34).
//Success means worker launch accepted, never a persisted-save receipt.
bool start_process_settings_job_v102(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
//LockTutorial uses job1. Start2 still executes original threadfun3/settings;
//the constructor's argument2 does not select a different worker body.
bool start_process_settings_job1_v118(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
//Modern worker error delivery on the Application thread, no source readiness
//or IsCurrectThreadRunning replacement (original returns1 unconditionally).
bool take_process_settings_job_error_v102(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
//Synchronous real saveSettings entry for the menu's actual NativeSaveSettings.
bool save_process_settings_v102(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
}
