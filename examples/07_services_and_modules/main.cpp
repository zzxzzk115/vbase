#include "../common.hpp"

#include <vbase/module/imodule.hpp>
#include <vbase/module/module_registry.hpp>
#include <vbase/patterns/service_registry.hpp>

using namespace vbase;

struct ILoggerService
{
    virtual void log(const std::string&) = 0;

    static constexpr auto serviceName() { return "ILogger"; }
};

class Logger : public IModule, public ILoggerService
{
public:
    Logger(ServiceRegistry& s) : m_ServiceRegistry(s) {}

    const char* name() const override { return "Logger"; }

    bool init() override
    {
        m_ServiceRegistry.provide<ILoggerService>(this);
        log("init");
        return true;
    }

    void shutdown() override { log("shutdown"); }

    void log(const std::string& m) override { std::cout << "[Log] " << m << "\n"; }

private:
    ServiceRegistry& m_ServiceRegistry;
};

class SomeOtherModule : public IModule
{
public:
    SomeOtherModule(ServiceRegistry& s) : m_ServiceRegistry(s) {}

    const char* name() const override { return "SomeOtherModule"; }

    bool init() override
    {
        auto& logger = m_ServiceRegistry.require<ILoggerService>();
        logger.log("SomeOtherModule init");
        return true;
    }

    void shutdown() override
    {
        auto& logger = m_ServiceRegistry.require<ILoggerService>();
        logger.log("SomeOtherModule shutdown");
    }

private:
    ServiceRegistry& m_ServiceRegistry;
};

int main()
{
    print_title("Services + Modules");

    ServiceRegistry serviceRegistry {};
    ModuleRegistry  moduleRegistry {};

    Logger logger(serviceRegistry);
    moduleRegistry.add(logger);

    SomeOtherModule som(serviceRegistry);
    moduleRegistry.add(som, {"Logger"});

    moduleRegistry.initAll();
    moduleRegistry.shutdownAll();

    return 0;
}