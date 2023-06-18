#pragma once

namespace round
{
template <typename T, typename U> constexpr inline T up(T number, U multiple)
{
  if (multiple % 2 == 0)
  {
    return (number + multiple - 1) & -multiple;
  }
  else
  {
    return (number + multiple - 1) / multiple * multiple;
  }
}
} // namespace round