#include "station.hpp"
#include <print>

namespace Winyl
{

void Station::ChangeState()
{
  if (this->IsPlaying)
  {
    std::println("station.cpp: TODO: Stop stream");
    // TODO: Stop a stream
  }
  else
  {
    std::println("station.cpp: TODO: Start stream");
    // TODO: Start a stream
  }

  this->IsPlaying = !this->IsPlaying;
}

} // namespace Winyl