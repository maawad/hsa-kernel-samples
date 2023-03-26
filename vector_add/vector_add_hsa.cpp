#include <hsa/hsa.h>
#include <hsa/hsa_ext_amd.h>
#include <hsa/hsa_ext_finalize.h>

#include <cstring>
#include <iostream>
#include <vector>

#define hsa_try(status)                                          \
  if (status != HSA_STATUS_SUCCESS) {                            \
    printf("%s:%d: error: 0x%x.\n", __FILE__, __LINE__, status); \
    exit(1);                                                     \
  }

class hsa_devices {
 public:
  hsa_devices() {
    // Initialize the agent handle
    hsa_iterate_agents(
        [](hsa_agent_t agent, void* data) -> hsa_status_t {
          hsa_devices* self = static_cast<hsa_devices*>(data);
          if (!self->valid_agent()) {
            self->agent_ = agent;
          }
          return HSA_STATUS_SUCCESS;
        },
        this);
  }

  bool valid_agent() const { return agent_.handle != 0; }

  void display_features() const {
    hsa_agent_feature_t features;
    hsa_agent_get_info(agent_, HSA_AGENT_INFO_FEATURE, &features);

    hsa_machine_model_t machine_model;
    hsa_agent_get_info(agent_, HSA_AGENT_INFO_MACHINE_MODEL, &machine_model);

    hsa_profile_t profile;
    hsa_agent_get_info(agent_, HSA_AGENT_INFO_PROFILE, &profile);

    uint16_t major_version;
    hsa_agent_get_info(agent_, HSA_AGENT_INFO_VERSION_MAJOR, &major_version);

    uint16_t minor_version;
    hsa_agent_get_info(agent_, HSA_AGENT_INFO_VERSION_MINOR, &minor_version);

    char name[64];
    hsa_agent_get_info(agent_, HSA_AGENT_INFO_NAME, name);

    uint32_t node;
    hsa_agent_get_info(agent_, HSA_AGENT_INFO_NODE, &node);

    uint32_t num_cus;
    // hsa_agent_get_info(agent_, HSA_AMD_AGENT_INFO_COMPUTE_UNIT_COUNT,
    // &num_cus);

    std::cout << "Agent Features: " << features << std::endl;
    std::cout << "Machine Model: " << machine_model << std::endl;
    std::cout << "Profile: " << profile << std::endl;
    std::cout << "Version: " << major_version << "." << minor_version
              << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Node ID: " << node << std::endl;
    std::cout << "Compute Unit Count: " << num_cus << std::endl;
  }
  hsa_agent_t get_agent() const { return agent_; }

 private:
  hsa_agent_t agent_;
};

int main() {
  const int n = 1024;
  std::vector<float> a(n);
  std::vector<float> b(n);
  std::vector<float> c(n);

  // Initialize input vectors
  for (int i = 0; i < n; ++i) {
    a[i] = i;
    b[i] = n - i;
  }

  hsa_devices agent_wrapper;
  if (agent_wrapper.valid_agent()) {
    agent_wrapper.display_features();
  } else {
    std::cerr << "Failed to find an HSA agent." << std::endl;
  }

  auto agent = agent_wrapper.get_agent();

  // Create the HSA program
  hsa_ext_program_t program;
  memset(&program, 0, sizeof(hsa_ext_program_t));
  hsa_ext_program_create(HSA_MACHINE_MODEL_LARGE, HSA_PROFILE_FULL,
                         HSA_DEFAULT_FLOAT_ROUNDING_MODE_DEFAULT, nullptr,
                         &program);

  // Assemble the HSA kernel
  const char* source = R"(
          __attribute__((hc))
          void vector_add(int* a, int* b, int* c, int n) {
              int i = hc_get_workitem_id(0);
              if (i < n) {
                  c[i] = a[i] + b[i];
              }
          }
      )";
  hsa_ext_module_t module;
  memset(&module, 0, sizeof(hsa_ext_module_t));
  hsa_ext_module_create(&program, HSA_MACHINE_MODEL_LARGE, HSA_PROFILE_FULL,
                        HSA_DEFAULT_FLOAT_ROUNDING_MODE_DEFAULT, nullptr,
                        &module);
  //   hsa_ext_module_add_code_from_memory(module, source, strlen(source) + 1);

  //   // Find the HSA vector_add kernel
  //   hsa_symbol_t symbol;
  //   hsa_ext_module_get_symbol(module, "&vector_add", &agent,
  //                             HSA_SYMBOL_KIND_KERNEL, &symbol);

  //   // Create the HSA queue
  //   uint32_t queue_size = 1024;
  //   hsa_queue_t* queue;
  //   hsa_queue_create(agent, queue_size, HSA_QUEUE_TYPE_MULTI, nullptr,
  //   nullptr,
  //                    UINT32_MAX, UINT32_MAX, &queue);
  //   // Allocate memory on the HSA device
  //   uint32_t num_bytes = n * sizeof(float);
  //   void *a_d, b_d, c_d;
  //   hsa_memory_allocate(agent, num_bytes, &a_d);
  //   hsa_memory_allocate(agent, num_bytes, &b_d);
  //   hsa_memory_allocate(agent, num_bytes, &c_d);

  //   // Copy the input vectors to the HSA device
  //   hsa_memory_copy(a_d, a.data(), num_bytes);
  //   hsa_memory_copy(b_d, b.data(), num_bytes);

  //   // Dispatch the kernel to the HSA device
  //   hsa_kernel_dispatch_packet_t packet = {0};
  //   packet.header =
  //       HSA_FENCE_SCOPE_SYSTEM << HSA_PACKET_HEADER_ACQUIRE_FENCE_SCOPE |
  //       HSA_FENCE_SCOPE_SYSTEM << HSA_PACKET_HEADER_RELEASE_FENCE_SCOPE;
  //   packet.header |= HSA_PACKET_TYPE_KERNEL_DISPATCH <<
  //   HSA_PACKET_HEADER_TYPE; packet.header |= 1 << HSA_PACKET_HEADER_BARRIER;
  //   packet.header |= 1 << HSA_PACKET_HEADER_EVENT;
  //   packet.kernel_object = symbol.handle;
  //   packet.private_segment_size = 0;
  //   packet.group_segment_size = 0;
  //   packet.kernel_code_entry_offset = 0;
  //   packet.workgroup_size_x = 256;
  //   packet.workgroup_size_y = 1;
  //   packet.workgroup_size_z = 1;
  //   packet.grid_size_x = (n + 255) / 256;
  //   packet.grid_size_y = 1;
  //   packet.grid_size_z = 1;
  //   packet.group_segment_offset = 0;
  //   packet.kernel_arguments_offset = 0;

  //   void* args[] = {&a_d, &b_d, &c_d, &n};
  //   uint32_t arg_sizes[] = {sizeof(void*), sizeof(void*), sizeof(void*),
  //                           sizeof(int)};
  //   packet.kernel_arguments_size = sizeof(args);

  //   hsa_memory_copy(
  //       queue->base_address + queue->write_index * queue->doorbell_stride,
  //       &packet, sizeof(packet));
  //   hsa_signal_t signal;
  //   hsa_signal_create(1, 0, nullptr, &signal);
  //   hsa_signal_store_relaxed(queue->doorbell_signal, queue->write_index);
  //   hsa_signal_wait_acquire(signal, HSA_SIGNAL_CONDITION_EQ, 0, UINT64_MAX,
  //                           HSA_WAIT_STATE_BLOCKED);
  //   hsa_signal_destroy(signal);

  //   // Copy the output vector from the HSA device
  //   hsa_memory_copy(c.data(), c_d, num_bytes);

  //   // Free the memory on the HSA device
  //   hsa_memory_free(a_d);
  //   hsa_memory_free(b_d);
  //   hsa_memory_free(c_d);

  //   // Destroy the HSA queue
  //   hsa_queue_destroy(queue);

  //   // Destroy the HSA program
  //   hsa_ext_program_destroy(program);

  //   // Shut down HSA
  //   hsa_shut_down();

  //   // Verify the results
  //   for (int i = 0; i < n; ++i) {
  //     if (c[i] != a[i] + b[i]) {
  //       std::cerr << "Error: c[" << i << "] = " << c[i] << " != "
  //                 << "a[" << i << "] + b[" << i << "] = " << a[i] + b[i] <<
  //                 "\n";
  //       return 1;
  //     }
  //   }
}