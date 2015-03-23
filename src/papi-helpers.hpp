#pragma once
#include <vector>
#include <string>

namespace papi{
struct event_info_t{
  int component;
  std::vector<int> codes;
  std::vector<std::string> names;
};

std::vector<event_info_t> init_papi_counters(const char** event_names,
                                             int num_event_names) {
  std::vector<event_info_t> eventsets;
  int retval;
  for(int i = 0; i < num_event_names; ++i){
    const char* event_name  = event_names[i];
    int event_code;
    retval = PAPI_event_name_to_code(const_cast<char*>(event_name), 
                                     &event_code);
    if (retval != PAPI_OK) {
      std::cerr << "Error: bad PAPI event name to code: ";
      PAPI_perror(NULL);
      exit(-1);
    }
    int component = PAPI_get_event_component(event_code);
    auto elem = find_if(
        begin(eventsets), end(eventsets),
        [&](const event_info_t& c) { return c.component == component; });
    if (elem == end(eventsets)) {
      event_info_t new_info;
      new_info.component = component;
      new_info.codes.push_back(event_code);
      new_info.names.emplace_back(event_name);
      eventsets.push_back(new_info);
    } else {
      elem->codes.push_back(event_code);
      elem->names.emplace_back(event_name);
    }
  }
  return eventsets;
}

std::vector<int> make_event_sets(const std::vector<event_info_t>& event_infos) {
  std::vector<int> eventsets;
  for (const auto& event : event_infos) {
    int eventset = PAPI_NULL;
    int retval = PAPI_create_eventset(&eventset);
    if (retval != PAPI_OK) {
      std::cerr << "Error: bad PAPI create eventset: ";
      PAPI_perror(NULL);
      exit(-1);
    }
    retval = PAPI_add_events(eventset, const_cast<int*>(&event.codes[0]),
                             event.codes.size());
    if (retval != PAPI_OK) {
      std::cerr << "Error: bad PAPI add eventset: ";
      PAPI_perror(NULL);
      exit(-1);
    }
    eventsets.push_back(eventset);
  }
  return eventsets;
}
}
