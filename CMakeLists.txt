add_custom_command(
  OUTPUT mmstrat.1
  COMMAND cat "${CMAKE_CURRENT_SOURCE_DIR}/mmstrat.wiki"
    | perl "${CMAKE_CURRENT_SOURCE_DIR}/wiki2pod"
    | pod2man --name=MMSTRAT --section=1 --center="Mastermind Strategies" 
      --release="mmstrat-1.0.0" 
    > mmstrat.1.tmp
  COMMAND test -s mmstrat.1.tmp && mv mmstrat.1.tmp mmstrat.1
  DEPENDS mmstrat.wiki)

add_custom_target(man ALL DEPENDS mmstrat.1)
