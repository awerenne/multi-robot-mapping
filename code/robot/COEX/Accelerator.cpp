#include "Accelerator.h"


//============
Accelerator::Accelerator(float alpha=0) {
    alpha = alpha;
    min_speed = 2.5;
    float nil;
    stop(nil);
}


//============
void Accelerator::start(float& progress_speed, const float& target_speed,
        const float& T) {
    is_running = true;
    if (progress_speed < min_speed) progress_speed = min_speed;
    this->target_speed = target_speed;
    float delta_speed = target_speed - progress_speed;
    this->T = T;
    this->A = delta_speed / T;
    this->B = A / (1-alpha);
    this->beta = alpha * T;
    this->start_t = millis();
    this->prev_t = 0;
}


//============
void Accelerator::stop(float& progress_speed) {
    is_running = false;
    progress_speed = min_speed;
}


//============
void Accelerator::accelerate(float& progress_speed) {
    current_t = ((float) (millis() - start_t))/1000.;
    if (!is_running || saturation(progress_speed)) return;
    if (alpha == 0) constantAcceleration(progress_speed);
    else smoothAcceleration(progress_speed);
    prev_t = current_t;
}


//============
bool Accelerator::saturation(const float& progress_speed) {
    return (A == 0) || (A > 0 && progress_speed >= target_speed) ||
           (A < 0 && progress_speed <= target_speed);
}


//============
void Accelerator::constantAcceleration(float& progress_speed) {
    progress_speed += uniformProfile() * (current_t - prev_t);
}


//============
void Accelerator::smoothAcceleration(float& progress_speed) {
    if (current_t < 1.05*T) 
        progress_speed += integral(current_t) - integral(prev_t);
    else progress_speed = target_speed;
}


//============
float Accelerator::uniformProfile() {
    return A;
}


//============
float Accelerator::trapezoidProfile(float t) {
    if (t <= beta) return (B/beta) * t;
    if (t <= (T-beta)) return B;
    return B - (B/beta) * (t-(T-beta));
}


//============
float Accelerator::integral(float t) {
    float integral_triangle = B * beta / 2;
    float integral_rectangle = B * (T - 2*beta);
    if (t <= beta) return trapezoidProfile(t) * t/2;
    if (t <= (T-beta)) return integral_triangle + (t-beta) * B;
    return 2*integral_triangle + integral_rectangle - \
            trapezoidProfile(t) * (T-t)/2;
}











