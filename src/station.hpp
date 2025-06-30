#pragma once
#include <string>

namespace Winyl
{

class Station
{
public:
  std::string Name;
  std::string Url;

  void ChangeState();

private:
  bool IsPlaying;
};

} // namespace Winyl