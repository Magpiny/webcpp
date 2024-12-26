#include <print>
#include <string_view>

int main()
{

    std::string_view plugin = "Plugin placeholder";
    std::println("This is a {}", plugin);

    return 0;
}
