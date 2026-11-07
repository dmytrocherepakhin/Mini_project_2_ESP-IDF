idf_component_register(
    SRCS
        "button.cpp"
        "indicator.cpp"
        "relay.cpp"
        "blink_clock.cpp"
        "event_dispatcher.cpp"
    INCLUDE_DIRS
        "include"
    PRIV_INCLUDE_DIRS "../../include"
    REQUIRES
        dl
)
