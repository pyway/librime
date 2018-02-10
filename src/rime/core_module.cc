//
// Copyright RIME Developers
// Distributed under the BSD License
//
// 2013-10-17 GONG Chen <chen.sst@gmail.com>
//

#include <rime_api.h>
#include <rime/common.h>
#include <rime/registry.h>

// built-in components
#include <rime/config.h>
#include <rime/config/plugins.h>
#include <rime/schema.h>

using namespace rime;

static void rime_core_initialize() {
  LOG(INFO) << "registering core components.";
  Registry& r = Registry::instance();

  auto config_builder = new ConfigComponent<ConfigBuilder>(
      [](ConfigBuilder* builder) {
        builder->InstallPlugin(new AutoPatchConfigPlugin);
        builder->InstallPlugin(new DefaultConfigPlugin);
        builder->InstallPlugin(new LegacyPresetConfigPlugin);
        builder->InstallPlugin(new LegacyDictionaryConfigPlugin);
        builder->InstallPlugin(new BuildInfoPlugin);
      });
  r.Register("config_builder", config_builder);

  //auto config_loader =
  //    new ConfigComponent<ConfigLoader>({"config", "build/", ".yaml"});
  r.Register("config", config_builder);
  r.Register("schema", new SchemaComponent(config_builder));

  auto user_config = new ConfigComponent<ConfigLoader>(
      [](ConfigLoader* loader) {
        loader->set_auto_save(true);
      });
  r.Register("user_config", user_config);
}

static void rime_core_finalize() {
  // registered components have been automatically destroyed prior to this call
}

RIME_REGISTER_MODULE(core)
