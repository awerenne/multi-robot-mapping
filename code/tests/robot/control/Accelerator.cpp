#include "Accelerator.h"


//============
Accelerator::Accelerator(float alpha=0) {
    this->alpha = alpha;
    this->stop();
}


//============
void Accelerator::start(float progress_speed, float target_speed, float T) {
    this->is_running = true;
    this->target_speed = target_speed;
    float delta_speed = target_speed - progress_speed;
    this->T = T;
    this->A = delta_speed / T;
    this->B = A / (1-this->alpha);
    this->start_t = millis();
    this->prev_t = 0;
    this->beta = this->alpha * T;
}


//============
void Accelerator::stop() {
    this->is_running = false;
}


//============
void Accelerator::accelerate(float& progress_speed) {
    this->current_t = ((float) (millis() - this->start_t))/1000;
    if (!this->is_running || this->saturation(progress_speed)) return;
    if (this->alpha == 0) this->constantAcceleration(progress_speed);
    else this->smoothAcceleration(progress_speed);
    this->prev_t = this->current_t;
}


//============
bool Accelerator::saturation(const float& progress_speed) {
    return (this->A == 0) || 
           (this->A > 0 && progress_speed >= this->target_speed) ||
           (this->A < 0 && progress_speed <= this->target_speed);
}


//============
void Accelerator::constantAcceleration(float& progress_speed) {
    progress_speed += this->uniformProfile() * (this->current_t - this->prev_t);
}


//============
void Accelerator::smoothAcceleration(float& progress_speed) {
    if (this->current_t <= T) {
        progress_speed += this->integralTrapezoidProfile(this->current_t) - \
                this->integralTrapezoidProfile(this->prev_t);
    }
    else  // Safety case to avoid theoretic "no reaching" case
        progress_speed = target_speed;
}


//============
float Accelerator::uniformProfile() {
    return this->A;
}


//============
float Accelerator::trapezoidProfile(float t) {
    if (t <= this->beta) return (this->B/this->beta) * t;
    if (t <= (T-this->beta)) return this->B;
    return this->B - (this->B/this->beta) * (t-(this->T-this->beta));
}


//============
float Accelerator::integralTrapezoidProfile(float t) {
    float integral_triangle = this->B * this->beta / 2;
    float integral_rectangle = this->B * (T - this->beta*2);
    if (t <= this->beta) return this->trapezoidProfile(t) * t/2;
    if (t <= (T - this->beta)) return integral_triangle + (t-this->beta) * this->B;
    return 2*integral_triangle + integral_rectangle - this->trapezoidProfile(t) * (T-t)/2;
}











