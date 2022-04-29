# SPDX-License-Identifier: XXX
# Copyright (C) 2021 XXX

include(colors)

macro(summary_header)
  string(APPEND _summary
    "${COLOR_BoldCyan}"
    "\n"
"
#------------------------------------------------------------------------------#
"
    "# Configuration Summary"
"
#------------------------------------------------------------------------------#
"
    "\n"
    "${COLOR_ColorReset}"
)
endmacro()

macro(summary_info name info allow_split)
  if(NOT ${info} STREQUAL "")
    string(REPLACE " " ";" split ${info})
    list(LENGTH split split_length)
    string(LENGTH ${name} name_length)

    string(APPEND _summary
      "${COLOR_Plain}"
      "  ${name}:"
      "${COLOR_Brown} "
    )

    if(${allow_split} AND split_length GREATER 1)
      math(EXPR split_minus "${split_length}-1")
      list(GET split ${split_minus} last)
      list(REMOVE_AT split ${split_minus})

      set(fill " ")
      string(LENGTH ${fill} fill_length)
      while(${fill_length} LESS ${name_length})
        string(APPEND fill " ")
        string(LENGTH ${fill} fill_length)
      endwhile()

      string(APPEND _summary "${COLOR_Brown}")
      foreach(entry ${split})
        string(APPEND _summary
          "${entry}\n${fill}    "
          )
      endforeach()
      string(APPEND _summary "${last}${COLOR_ColorReset}\n")
    else()
      string(APPEND _summary
        "${info}"
        "${COLOR_ColorReset}"
        "\n"
      )
    endif()
  endif()
endmacro()

macro(summary_option name state extra)
  string(APPEND _summary
    "${COLOR_Plain}"
    "  ${name}:"
    "${COLOR_ColorReset}"
  )

  if(${state})
    string(APPEND _summary
      "${COLOR_Green}"
      " ${state}"
      "${COLOR_ColorReset}"
      "${extra}"
    )
  else()
    string(APPEND _summary
      "${COLOR_BoldGrey}"
      " ${state}"
      "${COLOR_ColorReset}"
    )
  endif()
  string(APPEND _summary "\n")
endmacro()
