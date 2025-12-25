#pragma once

class SimulationConfig {
  public:
    static constexpr int TIMER_INTERVAL_MS = 16; // ~60 FPS

    static constexpr float DELTA_TIME_SECONDS = 0.016f; // ~60 FPS

    static int GetTimerIntervalMs() { return timer_interval_ms_; }
    static float GetDeltaTimeSeconds() { return delta_time_seconds_; }

    static void SetUpdateRate(int fps) {
        timer_interval_ms_ = 1000 / fps;
        delta_time_seconds_ = 1.0f / fps;
    }

  private:
    static inline int timer_interval_ms_ = TIMER_INTERVAL_MS;
    static inline float delta_time_seconds_ = DELTA_TIME_SECONDS;
};










