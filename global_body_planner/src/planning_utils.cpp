#include "global_body_planner/planning_utils.h"
#include "spirit_utils/matplotlibcpp.h"

namespace planning_utils {

namespace plt = matplotlibcpp;

State fullStateToState(FullState full_state)
{
  State state;
  double x = full_state[0];
  double y = full_state[1];
  double z = full_state[2];
  double dx = full_state[6];
  double dy = full_state[7];
  double dz = full_state[8];

  state[0] = x;
  state[1] = y;
  state[2] = z;
  state[3] = dx;
  state[4] = dy;
  state[5] = dz;

  return state;
}

FullState stateToFullState(State state, double roll, double pitch, double yaw, 
  double roll_rate, double pitch_rate, double yaw_rate)
{
  FullState full_state;
  full_state.resize(FULLSTATEDIM);

  full_state[0] = state[0];
  full_state[1] = state[1];
  full_state[2] = state[2];
  full_state[3] = roll;
  full_state[4] = pitch;
  full_state[5] = yaw;
  full_state[6] = state[3];
  full_state[7] = state[4];
  full_state[8] = state[5];

  // Convert euler rates to body angular velocity
  Eigen::Vector3d d_rpy,ang_vel;
  d_rpy << roll_rate, pitch_rate, yaw_rate;

  Eigen::Matrix3d d_rpy_to_ang_vel;
  d_rpy_to_ang_vel << cos(pitch)*cos(yaw), -sin(yaw), 0, 
                      cos(pitch)*sin(yaw),  cos(yaw), 0,
                              -sin(pitch),         0, 1;
  
  ang_vel = d_rpy_to_ang_vel*d_rpy;

  full_state[9] = ang_vel[0];
  full_state[10] = ang_vel[1];
  full_state[11] = ang_vel[2];

  return full_state;
}

void flipDirection(State &state)
{
  state[3] = -state[3];
  state[4] = -state[4];
  state[5] = -state[5];
};

void vectorToArray(State vec, double * new_array)
{
  for (int i = 0; i < vec.size(); i++)
    new_array[i] = vec.at(i);
}
void printState(State vec)
{
  printf("{");
  for (int i=0; i < vec.size(); i++)
    printf("%4.3f, ",vec[i]);
  printf("\b\b} ");
}
void printAction(Action a)
{
  std::cout << "{";
  for (int i= 0; i < a.size(); i++)
    std::cout << a[i] << ", ";
  std::cout << "\b\b}"; 
}
void printVector(std::vector<double> vec)
{
  std::cout << "{";
  for (double i= 0; i < vec.size(); i++)
    std::cout << vec[i] << ", ";
  std::cout << "\b\b}"; 
}
void printVectorInt(std::vector<int> vec)
{
  std::cout << "{";
  for (int i= 0; i < vec.size(); i++)
    std::cout << vec[i] << ", ";
  std::cout << "\b\b}"; 
}
void printStateNewline(State vec)
{
  printState(vec); std::cout << std::endl;
}
void printActionNewline(Action a)
{
  printAction(a); std::cout << std::endl;
}
void printVectorNewline(std::vector<double> vec)
{
  printVector(vec); std::cout << std::endl;
}
void printStateSequence(std::vector<State> state_sequence)
{
  for (State s : state_sequence)
    printStateNewline(s);
}
void printInterpStateSequence(std::vector<State> state_sequence, 
  std::vector<double> interp_t)
{
  for (int i=0;i<state_sequence.size();i++)
  {
    std::cout << interp_t[i] << "\t";
    printStateNewline(state_sequence[i]);
  }
}
void printActionSequence(std::vector<Action> action_sequence)
{
  for (Action a : action_sequence)
    printActionNewline(a);
}
void printVectorIntNewline(std::vector<int> vec)
{
  printVectorInt(vec); std::cout << std::endl;
}
void plotYaw(std::vector<double> interp_t,
  std::vector<FullState> interp_full_plan) {

  std::vector<double> yaw;
  std::vector<double> yaw_rate;

  for (FullState full_state : interp_full_plan) {
    yaw.push_back(full_state[5]);
    yaw_rate.push_back(full_state[11]);
  }

  // plt::clf();
  plt::ion();
  plt::named_plot("yaw", interp_t, yaw);
  plt::named_plot("yaw rate", interp_t, yaw_rate);
  plt::xlabel("t");
  plt::ylabel("yaw");
  plt::legend();
  plt::show();
  plt::pause(0.001);

}

double poseDistance(State q1, State q2)
{
  double sum = 0;
  for (int i = 0; i < POSEDIM; i++)
  {
    sum = sum + (q2[i] - q1[i])*(q2[i] - q1[i]);
  }

  double dist = sqrt(sum);
  return dist;
}

double poseDistance(std::vector<double> v1, std::vector<double> v2)
{
  double sum = 0;

  if (v1.size() < 3 || v2.size() < 3) {
    throw std::runtime_error("Not enough data in vectors to compute distance");
  }

  for (int i = 0; i < 3; i++)
  {
    sum = sum + (v2[i] - v1[i])*(v2[i] - v1[i]);
  }

  double dist = sqrt(sum);
  return dist;
}

double stateDistance(State q1,State q2)
{
  double sum = 0;
  State state_weight = {1,1,1,0.1,0.1,0.1};

  for (int i = 0; i < q1.size(); i++)
  {
    sum = sum + state_weight[i]*(q2[i] - q1[i])*(q2[i] - q1[i]);
  }

  double dist = sqrt(sum);
  return dist;
}

bool isWithinBounds(State s1, State s2, const PlannerConfig &planner_config)
{
  return (stateDistance(s1, s2) <= planner_config.GOAL_BOUNDS);
}

void addFullStates(FullState start_state, std::vector<State> interp_reduced_plan, double dt, 
  std::vector<FullState> &interp_full_plan, const PlannerConfig &planner_config) {

  int num_states = interp_reduced_plan.size();

  // Set roll and roll rate to zero
  double roll = 0;
  double roll_rate = 0;

  // Declare variables for yaw
  std::vector<double> z(num_states);
  std::vector<double> filtered_z(num_states);
  std::vector<double> z_rate(num_states);
  std::vector<double> filtered_z_rate(num_states);
  std::vector<double> pitch(num_states);
  std::vector<double> pitch_rate(num_states);
  std::vector<double> filtered_pitch_rate(num_states);
  std::vector<double> wrapped_yaw(num_states);
  std::vector<double> unwrapped_yaw(num_states);
  std::vector<double> filtered_yaw(num_states);
  std::vector<double> yaw_rate(num_states);
  std::vector<double> filtered_yaw_rate(num_states);

  // Enforce that yaw and pitch match current state, let the filter smooth things out
  z[0] = start_state[2];
  pitch[0] = start_state[4];
  wrapped_yaw[0] = start_state[5];

  // Compute wrapped yaw first to align with heading
  for (int i = 1; i < num_states; i++) {
    State body_state = interp_reduced_plan[i];
    wrapped_yaw[i] = atan2(body_state[4],body_state[3]);
  }

  // Unwrap yaw for filtering
  unwrapped_yaw = math_utils::unwrap(wrapped_yaw);
  
  // Compute pitch and height to align with the terrain, add first order filter on init state
  double gamma = 0.98;
  for (int i = 1; i < num_states; i++) {
    double weight = pow(gamma,i);
    State body_state = interp_reduced_plan[i];
    z[i] = weight*z[0] + (1-weight)*body_state[2];
    pitch[i] = weight*pitch[0] + (1-weight)*getPitchFromState(body_state,planner_config);
    unwrapped_yaw[i] = weight*unwrapped_yaw[0] + (1-weight)*unwrapped_yaw[i];
  }

  // Hold penultimate yaw value to avoid singularity issues at terminal state (often zero velocity)
  unwrapped_yaw[num_states-1] = unwrapped_yaw[num_states-2];

  // Filter yaw and compute its derivative via central difference method
  int window_size = 25;
  filtered_yaw = math_utils::movingAverageFilter(unwrapped_yaw, window_size);
  yaw_rate = math_utils::centralDiff(filtered_yaw, dt);
  filtered_yaw_rate = math_utils::movingAverageFilter(yaw_rate, window_size);
  pitch_rate = math_utils::centralDiff(pitch, dt);
  filtered_pitch_rate = math_utils::movingAverageFilter(pitch_rate,window_size);

  // Filter z with a much tighter window
  int z_window_size = 5;
  filtered_z = math_utils::movingAverageFilter(z, z_window_size);
  z_rate = math_utils::centralDiff(filtered_z, dt);
  filtered_z_rate = math_utils::movingAverageFilter(z_rate,z_window_size);

  std::vector<double> interp_t(num_states);
  for (int i = 0; i < num_states; i++) {
    interp_t[i] = i*dt;
  }

  // Wrap yaw again
  wrapped_yaw = math_utils::wrapToPi(unwrapped_yaw);
  filtered_yaw = math_utils::wrapToPi(filtered_yaw);

  // plt::clf();
  // plt::ion();
  // // plt::named_plot("unwrapped yaw", interp_t, unwrapped_yaw);
  // plt::named_plot("wrapped yaw", interp_t, wrapped_yaw);
  // plt::named_plot("filtered yaw", interp_t, filtered_yaw);
  // plt::named_plot("yaw rate", interp_t, yaw_rate);
  // plt::named_plot("filtered yaw rate", interp_t, filtered_yaw_rate);
  // plt::xlabel("t");
  // plt::ylabel("z");
  // plt::legend();
  // plt::show();
  // plt::pause(0.001);

  // Add full state data into the array
  std::vector<double> x_vec, y_vec, z_vec;
  for (int i = 0; i < num_states; i++) {
    State body_state = interp_reduced_plan[i];
    body_state[2] = filtered_z[i];
    body_state[8] = filtered_z_rate[i];
    FullState body_full_state = stateToFullState(body_state, roll, pitch[i],
      filtered_yaw[i], roll_rate, filtered_pitch_rate[i], filtered_yaw_rate[i]);

    interp_full_plan.push_back(body_full_state);
    x_vec.push_back(body_state[0]);
    y_vec.push_back(body_state[1]);
    z_vec.push_back(body_state[2]);
  }

  // Plot the trajectories for debugging
  #ifdef PLOT_TRAJECTORIES
    plt::clf();
    plt::ion();
    plt::named_plot("z", interp_t, z);
    plt::named_plot("filtered z", interp_t, filtered_z);
    plt::named_plot("z rate", interp_t, z_rate);
    plt::named_plot("filtered z rate", interp_t, filtered_z_rate);
    plt::xlabel("t");
    plt::ylabel("z");
    plt::legend();
    plt::show();
    plt::pause(0.001);

    plt::clf();
    plt::ion();
    // plt::named_plot("unwrapped yaw", interp_t, unwrapped_yaw);
    plt::named_plot("wrapped yaw", interp_t, wrapped_yaw);
    plt::named_plot("filtered yaw", interp_t, filtered_yaw);
    plt::named_plot("yaw rate", interp_t, yaw_rate);
    plt::named_plot("filtered yaw rate", interp_t, filtered_yaw_rate);
    plt::xlabel("t");
    plt::ylabel("z");
    plt::legend();
    plt::show();
    
    plt::clf();
    plt::ion();
    plt::named_plot("x", interp_t, x_vec);
    plt::named_plot("y", interp_t, y_vec);
    plt::named_plot("z", interp_t, z_vec);
    plt::xlabel("t");
    plt::ylabel("data");
    plt::legend();
    plt::show();
    plt::pause(0.001);
  #endif
}

GRF getGRF(Action a,double t, const PlannerConfig &planner_config) {

  double m = planner_config.M_CONST;
  double g = planner_config.G_CONST;
  double t_s = a[6];

  GRF grf;

  // If in stance fill grf with appropriate data, otherwise leave as zeros
  if (t<=t_s) {
    // Get corresponding forces
    grf[0] = m*(a[0] + (a[3]-a[0])*t/t_s);
    grf[1] = m*(a[1] + (a[4]-a[1])*t/t_s);
    grf[2] = m*((a[2] + (a[5]-a[2])*t/t_s) + g);
  } else {
    grf = {0,0,0};
  }
  
  return grf;

}

Eigen::Vector3d getAcceleration(State s, Action a, double t)
{
    Eigen::Vector3d acc;

    double a_x_td = a[0]; // Acceleration in x at touchdown
    double a_y_td = a[1];
    double a_z_td = a[2];
    double a_x_to = a[3]; // Acceleration in x at takeoff
    double a_y_to = a[4];
    double a_z_to = a[5];
    double t_s = a[6];

    acc(0) = a_x_td + (a_x_to - a_x_td)*t/(t_s);
    acc(1) = a_y_td + (a_y_to - a_y_td)*t/(t_s);
    acc(2) = a_z_td + (a_z_to - a_z_td)*t/(t_s);
    return acc;
}

bool isValidYawRate(State s, Action a, double t, const PlannerConfig &planner_config)
{
  Eigen::Vector3d acc = getAcceleration(s, a, t);
  State s_next = applyStance(s,a,t,planner_config);

  double dy = s_next[4];
  double dx = s_next[3];
  double ddy = acc.y();
  double ddx = acc.x();
  double d_yaw;

  // Don't limit if moving slowly
  if ((dx*dx + dy*dy) <= 0.25)
  {
      d_yaw = 0;
      // std::cout << "Not moving, pass" << std::endl;
  } else {
      d_yaw = (dy*ddx - dx*ddy)/(dx*dx + dy*dy);
  }

  if (abs(d_yaw) > planner_config.DY_MAX)
  {
      return false;
  } else {
      return true;
  }
}

double getPitchFromState(State s, const PlannerConfig &planner_config) {

  std::array<double, 3> surf_norm = planner_config.terrain.getSurfaceNormalFiltered(s[0], s[1]);

  double denom = s[3]*s[3] + s[4]*s[4];

  if (denom <= 0 || surf_norm[2] <=0) {
    double default_pitch = 0;
    return default_pitch;
  } else {

    double v_proj = (s[3]*surf_norm[0] + s[4]*surf_norm[1])/
      sqrt(denom);

    double pitch = atan2(v_proj,surf_norm[2]);

    return pitch;
  }
}

double getHeightFromState(State s, const PlannerConfig &planner_config) {

  return (planner_config.terrain.getGroundHeightFiltered(s[0], s[1]));

}

void interpStateActionPair(State s, Action a,double t0,double dt, 
  std::vector<State> &interp_reduced_plan, std::vector<GRF> &interp_GRF,
  std::vector<double> &interp_t, std::vector<int> &interp_primitive_id,
  std::vector<double> &interp_length, const PlannerConfig &planner_config)
{
  double t_s = a[6];
  double t_f = a[7];

  // Add points during stance phase
  for (double t = 0; t < t_s; t += dt)
  {
    interp_t.push_back(t0+t);
    State s_next = applyStance(s,a,t,planner_config);
    if (!interp_reduced_plan.empty()) {
      interp_length.push_back(interp_length.back() + poseDistance(s_next, interp_reduced_plan.back()));
    }
    interp_reduced_plan.push_back(s_next);
    interp_GRF.push_back(getGRF(a,t, planner_config));

    if (!isValidYawRate(s,a,t,planner_config)) {
      std::cout << "Invalid yaw detected!" << std::endl;
      printStateNewline(s);
      printActionNewline(a);
      std::cout << "t = " << t<< std::endl;
      std::cout << "t_f = " << t_f<< std::endl;
    }

    if (t_f==0)
      interp_primitive_id.push_back(CONNECT_STANCE);
    else
      interp_primitive_id.push_back(STANCE);
  }

  State s_takeoff = applyStance(s, a, planner_config);
  // // Include the exact moment of liftoff if flight phase exists
  // if (t_f>0) {
  //   interp_t.push_back(t0+t_s);
  //   interp_reduced_plan.push_back(s_takeoff);
  //   interp_GRF.push_back(getGRF(a,t_s,planner_config));
  //   interp_primitive_id.push_back(STANCE);
  // }

  // Include the remainder of the flight phase (double count takeoff state to
  // get discontinuous dropoff in grf when interpolating)
  for (double t = 0; t < t_f; t += dt)
  {
    interp_t.push_back(t0+t_s+t);
    State s_next = applyFlight(s_takeoff, t);
    interp_length.push_back(interp_length.back() + poseDistance(s_next, interp_reduced_plan.back()));
    interp_reduced_plan.push_back(s_next);
    GRF grf = {0,0,0};
    interp_GRF.push_back(grf);
    interp_primitive_id.push_back(FLIGHT);
  }

  // // Include the exact landing state if there is a flight phase
  // if (t_f>0)
  // {
  //   interp_t.push_back(t_s+t_f+t0);
  //   interp_path.push_back(applyFlight(s_takeoff, t_f));
  //   interp_GRF.push_back(getGRF(a,t_s+t_f));
  //   interp_primitive_id.push_back(STANCE);
  // }
}

void getInterpPlan(FullState start_state, std::vector<State> state_sequence,
  std::vector<Action> action_sequence,double dt, double t0,
  std::vector<FullState> &interp_full_plan, std::vector<GRF> &interp_GRF, 
  std::vector<double> &interp_t, std::vector<int> &interp_primitive_id,
  std::vector<double> &interp_length, const PlannerConfig &planner_config)
{
  std::vector<State> interp_reduced_plan;

  // Loop through state action pairs, interp each and add to the path
  for (int i=0; i < action_sequence.size();i++)
  {
    interpStateActionPair(state_sequence[i], action_sequence[i], t0, dt, interp_reduced_plan,
      interp_GRF, interp_t, interp_primitive_id, interp_length, planner_config);
    t0 += (action_sequence[i][6] + action_sequence[i][7]);
  }

  // Add the final state in case it was missed by interp (GRF is undefined 
  // here so just copy the last element)
  interp_t.push_back(t0);
  interp_length.push_back(interp_length.back() + poseDistance(state_sequence.back(), interp_reduced_plan.back()));
  interp_reduced_plan.push_back(state_sequence.back());
  interp_GRF.push_back(interp_GRF.back());
  interp_primitive_id.push_back(STANCE);

  // Lift from reduced into full body plan
  addFullStates(start_state, interp_reduced_plan, dt, interp_full_plan, planner_config);

}

std::array<double,3> rotateGRF(std::array<double,3> surface_norm,
  std::array<double,3> grf)
{
  // Receive data and convert to Eigen
  Eigen::Vector3d Zs;
  Zs << 0,0,1;

  Eigen::Vector3d surface_norm_eig;
  surface_norm_eig << surface_norm[0],surface_norm[1],surface_norm[2];

  // Normalize surface normal
  // surface_norm_eig.normalize();

  Eigen::Vector3d grf_eig;
  grf_eig << grf[0],grf[1],grf[2];

  // Compute priors
  Eigen::Vector3d v = surface_norm_eig.cross(Zs);
  double s = v.norm();
  double c = surface_norm_eig.dot(Zs);

  Eigen::Matrix3d vskew;
  vskew << 0, -v[2], v[1],
       v[2], 0, -v[0],
       -v[1], v[0], 0;

  Eigen::Matrix3d R; // Rotation matrix to rotate from contact frame to spatial frame
  double eps = 1e-6;
  if (s < eps)
  {
    R = Eigen::Matrix3d::Identity();
  }
  else
  {
    R = Eigen::Matrix3d::Identity() + vskew + vskew*vskew * (1-c)/(s*s);
  }

  Eigen::Vector3d grf_spatial_eig = R*grf_eig;
  std::array<double,3> grf_spatial = {grf_spatial_eig[0],grf_spatial_eig[1],grf_spatial_eig[2]};

  return grf_spatial;
}

State applyStance(State s, Action a, double t, const PlannerConfig &planner_config)
{
  double a_x_td = a[0]; // Acceleration in x at touchdown
  double a_y_td = a[1];
  double a_z_td = a[2];
  double a_x_to = a[3]; // Acceleration in x at takeoff
  double a_y_to = a[4];
  double a_z_to = a[5];
  double t_s = a[6];

  double x_td = s[0];
  double y_td = s[1];
  double z_td = s[2];
  double dx_td = s[3];
  double dy_td = s[4];
  double dz_td = s[5];

  State s_new;

  s_new[0] = x_td + dx_td*t + 0.5*a_x_td*t*t + (a_x_to - a_x_td)*(t*t*t)/(6.0*t_s);
  s_new[1] = y_td + dy_td*t + 0.5*a_y_td*t*t + (a_y_to - a_y_td)*(t*t*t)/(6.0*t_s);
  s_new[2] = z_td + dz_td*t + 0.5*a_z_td*t*t + (a_z_to - a_z_td)*(t*t*t)/(6.0*t_s);
  s_new[3] = dx_td + a_x_td*t + (a_x_to - a_x_td)*t*t/(2.0*t_s);
  s_new[4] = dy_td + a_y_td*t + (a_y_to - a_y_td)*t*t/(2.0*t_s);
  s_new[5] = dz_td + a_z_td*t + (a_z_to - a_z_td)*t*t/(2.0*t_s);

  if (a[7] == 0) {
    s_new[2] = a_z_td + (a_z_to - a_z_td)*(t/t_s) + getHeightFromState(s_new, planner_config);
    // s_new[2] = getHeightFromState(s_new, planner_config);
    s_new[5] = 0;//sqrt(s_new[3]*s_new[3] + s_new[4]*s_new[4])*sin();
  } else {
    double g = planner_config.G_CONST;
    double peak_grf_x = a[0];
    double peak_grf_y = a[1];
    double peak_grf_z = a[2];
    double x0 = s[0];
    double y0 = s[1];
    double z0 = s[2];
    double dx0 = s[3];
    double dy0 = s[4];
    double dz0 = a[3];

    s_new[0] = x0 + dx0*t - ((g*peak_grf_x*pow(t,4))/3.0 - 
      (2*g*peak_grf_x*t_s*pow(t,3))/3.0)/pow(t_s,2);
    s_new[1] = y0 + dy0*t - ((g*peak_grf_y*pow(t,4))/3.0 - 
      (2*g*peak_grf_y*t_s*pow(t,3))/3.0)/pow(t_s,2);
    s_new[2] = z0 - ((g*peak_grf_z*pow(t,4))/3.0 - (2*g*peak_grf_z*t_s*pow(t,3))/3.0)/pow(t_s,2) + 
      (t*(6*dz0 - 3*g*t))/6.0;
    s_new[3] = dx0 - (2*g*peak_grf_x*pow(t,2)*(2*t - 3*t_s))/(3.0*pow(t_s,2));
    s_new[4] = dy0 - (2*g*peak_grf_y*pow(t,2)*(2*t - 3*t_s))/(3.0*pow(t_s,2));
    s_new[5] = dz0 - (g*t*(4.0*peak_grf_z*pow(t,2) - 6.0*peak_grf_z*t*t_s + 3.0*pow(t_s,2)))/
      (3.0*pow(t_s,2));
  }

  return s_new;
}

State applyStance(State s, Action a, const PlannerConfig &planner_config)
{
  return applyStance(s, a, a[6], planner_config);
}

State applyFlight(State s, double t_f)
{
  double g = 9.81;
  double x_to = s[0];
  double y_to = s[1];
  double z_to = s[2];
  double dx_to = s[3];
  double dy_to = s[4];
  double dz_to = s[5];

  State s_new;
  s_new[0] = x_to + dx_to*t_f;
  s_new[1] = y_to + dy_to*t_f;
  s_new[2] = z_to + dz_to*t_f - 0.5*g*t_f*t_f;
  s_new[3] = dx_to;
  s_new[4] = dy_to;
  s_new[5] = dz_to - g*t_f;

  return s_new;
}

State applyAction(State s, Action a, const PlannerConfig &planner_config)
{
  double t_s;
  double t_f;

  t_s = a[6];
  t_f = a[7];

  State s_to = applyStance(s, a, planner_config);
  State s_new = applyFlight(s_to, t_f);
  return s_new;
}

State applyActionReverse(State s, Action a, const PlannerConfig &planner_config)
{
  double t_s;
  double t_f;

  t_s = a[6];
  t_f = a[7];

  State s_to = applyFlight(s, -t_f);
  State s_new = applyStanceReverse(s_to, a, planner_config);
  return s_new;
}

State applyStanceReverse(State s, Action a, double t, const PlannerConfig &planner_config)
{
  double a_x_td = a[0];
  double a_y_td = a[1];
  double a_z_td = a[2];
  double a_x_to = a[3];
  double a_y_to = a[4];
  double a_z_to = a[5];
  double t_s = a[6];

  double x_to = s[0];
  double y_to = s[1];
  double z_to = s[2];
  double dx_to = s[3];
  double dy_to = s[4];
  double dz_to = s[5];

  State s_new;

  double cx = dx_to - a_x_td*t_s - 0.5*(a_x_to - a_x_td)*t_s;
  double cy = dy_to - a_y_td*t_s - 0.5*(a_y_to - a_y_td)*t_s;
  double cz = dz_to - a_z_td*t_s - 0.5*(a_z_to - a_z_td)*t_s;

  s_new[3] = dx_to - a_x_td*(t_s - t) - (a_x_to - a_x_td)*(t_s*t_s - t*t)/(2.0*t_s);
  s_new[4] = dy_to - a_y_td*(t_s - t) - (a_y_to - a_y_td)*(t_s*t_s - t*t)/(2.0*t_s);
  s_new[5] = dz_to - a_z_td*(t_s - t) - (a_z_to - a_z_td)*(t_s*t_s - t*t)/(2.0*t_s);
  s_new[0] = x_to - cx*(t_s - t) - 0.5*a_x_td*(t_s*t_s - t*t) - (a_x_to - a_x_td)*(t_s*t_s*t_s - t*t*t)/(6.0*t_s);
  s_new[1] = y_to - cy*(t_s - t) - 0.5*a_y_td*(t_s*t_s - t*t) - (a_y_to - a_y_td)*(t_s*t_s*t_s - t*t*t)/(6.0*t_s);
  s_new[2] = z_to - cz*(t_s - t) - 0.5*a_z_td*(t_s*t_s - t*t) - (a_z_to - a_z_td)*(t_s*t_s*t_s - t*t*t)/(6.0*t_s);

  if (a[7] == 0) {
    s_new[2] = a_z_to + (a_z_td - a_z_to)*(t/t_s) + getHeightFromState(s_new, planner_config);
    // s_new[2] = getHeightFromState(s_new, planner_config);
    s_new[5] = 0;//sqrt(s_new[3]*s_new[3] + s_new[4]*s_new[4])*sin();
  } else {
    double g = planner_config.G_CONST;
    double peak_grf_x = a[0];
    double peak_grf_y = a[1];
    double peak_grf_z = a[2];
    double x0 = s[0];
    double y0 = s[1];
    double z0 = s[2];
    double dx0 = s[3];
    double dy0 = s[4];
    double dz0 = s[5];
    
    s_new[0] = -(g*peak_grf_x*pow(t,4))/(3*pow(t_s,2)) + 
      (2*g*peak_grf_x*pow(t,3))/(3*t_s) + (dx0 - (2*g*peak_grf_x*t_s)/3.0)*t + 
      x0 - t_s*(dx0 - (2*g*peak_grf_x*t_s)/3.0) - (g*peak_grf_x*pow(t_s,2))/3.0;
    s_new[1] = -(g*peak_grf_y*pow(t,4))/(3*pow(t_s,2)) + 
      (2*g*peak_grf_y*pow(t,3))/(3*t_s) + (dy0 - (2*g*peak_grf_y*t_s)/3.0)*t + 
      y0 - t_s*(dy0 - (2*g*peak_grf_y*t_s)/3.0) - (g*peak_grf_y*pow(t_s,2))/3.0;
    s_new[2] = z0 - ((g*peak_grf_z*pow(t,4))/3.0 - (2*g*peak_grf_z*t_s*pow(t,3))/3.0)/pow(t_s,2) + 
      (t*(6*dz0 - 3*g*t))/6.0 - (t_s*(6*dz0 - 3*g*t_s))/6.0 - (pow(t_s,2)*(6*g - 4*g*peak_grf_z))/6.0 - 
      (g*peak_grf_z*pow(t_s,2))/3.0 + (t*t_s*(6*g - 4*g*peak_grf_z))/6.0;
    s_new[3] = dx0 - (2*g*peak_grf_x*t_s)/3.0 - (2*g*peak_grf_x*pow(t,2)*(2*t - 3*t_s))/(3*pow(t_s,2));
    s_new[4] = dy0 - (2*g*peak_grf_y*t_s)/3.0 - (2*g*peak_grf_y*pow(t,2)*(2*t - 3*t_s))/(3*pow(t_s,2));
    s_new[5] = dz0 - (g*(2*peak_grf_z*pow(t_s,2) - 3*pow(t_s,2)))/(3*t_s) - (g*t*(4*peak_grf_z*pow(t,2) - 6*peak_grf_z*t*t_s + 3*pow(t_s,2)))/(3*pow(t_s,2));
  }

  return s_new;
}

State applyStanceReverse(State s, Action a, const PlannerConfig &planner_config)
{
  return applyStanceReverse(s, a, 0, planner_config);
}

Action getRandomAction(std::array<double, 3> surf_norm, const PlannerConfig &planner_config)
{ 
  Action a;

  // Random normal forces between 0 and planner_config.F_MAX
  double f_z_td = (planner_config.F_MAX-planner_config.F_MIN)*(double)rand()/RAND_MAX
    + planner_config.F_MIN;
  double f_z_to = (planner_config.F_MAX-planner_config.F_MIN)*(double)rand()/RAND_MAX
    + planner_config.F_MIN;

  // Random tangential forces between -mu*f_z and mu*f_z
  double f_x_td = 2*planner_config.MU*f_z_td*(double)rand()/RAND_MAX - planner_config.MU*f_z_td;
  double f_x_to = 2*planner_config.MU*f_z_to*(double)rand()/RAND_MAX - planner_config.MU*f_z_to;
  double f_y_td = 2*planner_config.MU*f_z_td*(double)rand()/RAND_MAX - planner_config.MU*f_z_td;
  double f_y_to = 2*planner_config.MU*f_z_to*(double)rand()/RAND_MAX - planner_config.MU*f_z_to;

  std::array<double, 3> f_td = {f_x_td, f_y_td, f_z_td};
  std::array<double, 3> f_to = {f_x_to, f_y_to, f_z_to};

  f_td = rotateGRF(surf_norm, f_td);
  f_to = rotateGRF(surf_norm, f_to);

  // Random stance and flight times between 0 and T_MAX
  // double t_s = (planner_config.T_S_MAX - planner_config.T_S_MIN)*(double)rand()/RAND_MAX + planner_config.T_S_MIN;
  double t_s = 0.3;
  double t_f = (planner_config.T_F_MAX - planner_config.T_F_MIN)*(double)rand()/RAND_MAX + planner_config.T_F_MIN;

  a[0] = f_td[0]/planner_config.M_CONST;
  a[1] = f_td[1]/planner_config.M_CONST;
  a[2] = f_td[2]/planner_config.M_CONST - planner_config.G_CONST;
  a[3] = f_to[0]/planner_config.M_CONST;
  a[4] = f_to[1]/planner_config.M_CONST;
  a[5] = f_to[2]/planner_config.M_CONST - planner_config.G_CONST;
  a[6] = t_s;
  a[7] = t_f;

  return a;
}

Action getRandomLeapAction(const State &s, std::array<double, 3> surf_norm,
  const PlannerConfig &planner_config)
{ 
  Action a;

  // Random normal forces between 0 and planner_config.F_MAX
  double peak_grf_z = (planner_config.PEAK_GRF_MAX-planner_config.PEAK_GRF_MIN)*(double)rand()/
    RAND_MAX + planner_config.PEAK_GRF_MIN;
  double dz0 = (double)rand()/RAND_MAX - 1.0;
  double dzf = (double)rand()/RAND_MAX - 1.0;

  // dz0 = -0.5;
  // peak_grf_z = 4.0;

  // Random tangential forces between -mu*f_z and mu*f_z
  double peak_grf_x = 2*planner_config.MU*peak_grf_z*(double)rand()/RAND_MAX -
    planner_config.MU*peak_grf_z;
  double peak_grf_y = 2*planner_config.MU*peak_grf_z*(double)rand()/RAND_MAX -
    planner_config.MU*peak_grf_z;

  // std::array<double, 3> f_td = {f_x_td, f_y_td, f_z_td};
  // std::array<double, 3> f_to = {f_x_to, f_y_to, f_z_to};

  // f_td = rotateGRF(surf_norm, f_td);
  // f_to = rotateGRF(surf_norm, f_to);

  // Random stance and flight times between 0 and T_MAX
  // double t_s = (planner_config.T_S_MAX - planner_config.T_S_MIN)*(double)rand()/RAND_MAX + planner_config.T_S_MIN;
  double z0 = s[2];
  double z_max = getHeightFromState(s, planner_config) + planner_config.H_MAX - 0.025;
  double g = planner_config.G_CONST;

  double t_s = (3.0*(dz0 - sqrt(pow(dz0,2) + 2*g*z0 - 2*g*z_max - (4*g*peak_grf_z*z0)/3.0 +
    (4*g*peak_grf_z*z_max)/3.0)))/(3*g - 2*g*peak_grf_z);

  double t_f = 1.0;

  // t_s = 0.2;
  t_f = 1.0;

  a[0] = peak_grf_x;
  a[1] = peak_grf_y;
  a[2] = peak_grf_z;
  a[3] = dz0;
  a[4] = dzf;
  a[5] = 0;
  a[6] = t_s;
  a[7] = t_f;

  return a;
}

bool isValidAction(Action a, const PlannerConfig &planner_config)
{
   
  if ((a[6] <= 0) || (a[7] < 0))
    return false;

  double m = planner_config.M_CONST;
  double g = planner_config.G_CONST;
  double mu = planner_config.MU;

  // Get corresponding forces
  double f_x_td = m*a[0];
  double f_y_td = m*a[1];
  double f_z_td = m*(a[2] + g);
  double f_x_to = m*a[3];
  double f_y_to = m*a[4];
  double f_z_to = m*(a[5] + g);

  // Check force limits
  if ((sqrt(f_x_td*f_x_td + f_y_td*f_y_td + f_z_td*f_z_td) >= planner_config.F_MAX) || 
    (sqrt(f_x_to*f_x_to + f_y_to*f_y_to + f_z_to*f_z_to) >= planner_config.F_MAX) ||
    (f_z_td < 0) || (f_z_to < 0) )
  {
    // std::cout << "Force limits violated" << std::endl;
    return false;
  }

  // Check friction cone
  if ((sqrt(f_x_td*f_x_td + f_y_td*f_y_td) >= mu*f_z_td) || 
    (sqrt(f_x_to*f_x_to + f_y_to*f_y_to) >= mu*f_z_to))
  {
    // std::cout << "Friction cone violated" << std::endl;
    return false;
  }

  return true;
}

bool isValidState(State s, const PlannerConfig &planner_config, int phase)
{
  if (planner_config.terrain.isInRange(s[0], s[1]) == false)
    return false;

  if (sqrt(s[3]*s[3] + s[4]*s[4]) > planner_config.V_MAX) // Ignore limit on vertical velocity since this is accurately bounded by gravity
    return false;

  // Find yaw, pitch, and their sines and cosines
  double yaw = atan2(s[4],s[3]); double cy = cos(yaw); double sy = sin(yaw);
  
  double pitch = getPitchFromState(s,planner_config);
  double cp = cos(pitch); double sp = sin(pitch);  

  // Compute each element of the rotation matrix
  double R_11 = cy*cp; double R_12 = -sy; double R_13 = cy*sp;
  double R_21 = sy*cp; double R_22 = cy;  double R_23 = sy*sp;
  double R_31 = -sp;   double R_32 = 0;   double R_33 = cp; 

  std::array<double, 2> test_x = {-0.5*planner_config.ROBOT_L, 0.5*planner_config.ROBOT_L};
  std::array<double, 2> test_y = {-0.5*planner_config.ROBOT_W, 0.5*planner_config.ROBOT_W};
  double z_body = -planner_config.ROBOT_H;

  // Check each of the four corners of the robot
  for (double x_body : test_x)
  {
    for (double y_body : test_y)
    {
      double x_leg = s[0] + R_11*x_body + R_12*y_body;
      double y_leg = s[1] + R_21*x_body + R_22*y_body;
      double z_leg = s[2] + R_31*x_body + R_32*y_body;

      double x_corner = x_leg + R_13*z_body;
      double y_corner = y_leg + R_23*z_body;
      double z_corner = z_leg + R_33*z_body;

      double leg_height = z_leg - planner_config.terrain.getGroundHeight(x_leg,y_leg);
      double corner_height = z_corner - 
        planner_config.terrain.getGroundHeight(x_corner,y_corner);

      // Check for collision
      if (corner_height < planner_config.H_MIN)
        return false;
      
      // Check for reachability
      if (phase == CONNECT_STANCE) {
        if (leg_height > planner_config.H_MAX) {
          return false;
        }
      } else if (phase == STANCE) {
        if (leg_height > planner_config.H_MAX) {
          return false;
        }
      }
    }
  }

  // Check the center of the underside of the robot for penetration
  double height = (s[2] + R_33*z_body) - planner_config.terrain.getGroundHeight(s[0] + 
    R_13*z_body,s[1] + R_23*z_body);
  if (height < planner_config.H_MIN)
    return false;


  return true;
}

bool isValidStateActionPair(State s, Action a, StateActionResult &result,
  const PlannerConfig &planner_config)
{

  std::cout << "Entering isValidStateActionPair" << std::endl;
  
  // Declare stance and flight times
  double t_s = a[6];
  double t_f = a[7];

  // Declare states, actions, and lengths for upcoming validity and distance checks
  State s_prev = s;
  State s_next;
  result.length = 0;
  result.a_new = a;

  int phase;
  if (t_f == 0) {
    phase = CONNECT_STANCE;
  } else {
    phase = STANCE;
  }

  for (double t = 0; t <= t_s; t += planner_config.KINEMATICS_RES)
  {
    // Compute state to check
    State s_next = applyStance(s,a,t,planner_config);

    if (isValidState(s_next, planner_config, phase) == false || (!isValidYawRate(s,a,t,planner_config)))
    {
      result.t_new = (1.0-planner_config.BACKUP_RATIO)*t;
      result.s_new = applyStance(s,a,result.t_new,planner_config);
      result.a_new[7] = 0.001;
      // s_new = applyStance(s,a,(t - planner_config.BACKUP_TIME));
      std::cout << "Invalid stance config" << std::endl;
      return false;
    } else {
      result.t_new = t;
      result.s_new = s_next;
      result.length += poseDistance(s_next, s_prev);
      s_prev = s_next;
    }
  }

  State s_takeoff = applyStance(s, a,planner_config);

  if (!isValidState(s_takeoff, planner_config, STANCE)) {
    result.t_new = (1.0-planner_config.BACKUP_RATIO)*t_s;
    result.s_new = applyStance(s,a,result.t_new,planner_config);
    std::cout << "Invalid s_takeoff config" << std::endl;
    return false;
  }

  double landing_height = 0.3;
  double t_f_land = 0;
  State s_land = s_takeoff;
  for (double t = planner_config.KINEMATICS_RES; t < t_f; t += planner_config.KINEMATICS_RES)
  {
    State s_next = applyFlight(s_takeoff, t);
    result.length += poseDistance(s_next, s_prev);
    result.t_new = t_s+t;
    result.a_new[7] = t;
    result.a_new[4] = s_next[5];
    result.s_new = s_next;
     
    // If newly within landing height, update landing state
    if (s_prev[2] - getHeightFromState(s_prev, planner_config) > landing_height &&
        s_next[2] - getHeightFromState(s_next, planner_config) <= landing_height){
      t_f_land = t;
      s_land = s_next;
      result.s_new = s_land;
    }

    // Check if landing collision has been detected
    if (t_f_land > 0) {
      if (!isValidState(s_next, planner_config, STANCE)) {

        // Accept if landing and collision are far enough away, otherwise reject
        if (planner_config.PEAK_GRF_MAX*planner_config.G_CONST > 
            pow(getSpeed(s_land),2)/(2*poseDistance(s_land,s_next)) &&
            isValidState(s_land, planner_config, STANCE)) {

          result.a_new[7] = t_f_land;
          result.a_new[4] = s_land[5];
          result.s_new = s_land;
          std::cout << "Valid landing config, success" << std::endl;
          return true;
        } else {
          std::cout << "Invalid landing config" << std::endl;
          return false;
        }
      }
    }

    // Check collision in flight
    if (!isValidState(s_next, planner_config, FLIGHT)) {
      return false;
    }

    s_prev = s_next;
  }

  if (t_f == 0) {
    std::cout << "t_f = 0 and valid, exiting " << std::endl;
    return true;
  } else {
    std::cout << "Did not find landing state, exiting " << std::endl;
    return false;
  }

  // // Check the exact landing state
  // State s_land = applyFlight(s_takeoff, t_f);
  // if (isValidState(s_land, planner_config, phase) == false)
  // {
  //   return false;
  // } else {
  //   result.t_new = t_s+t_f;
  //   result.s_new = s_land;
  //   result.length += poseDistance(s_land, s_prev);
  // }

  // If both stance and flight trajectories are entirely valid, return true
  // return true;
}

bool isValidStateActionPairReverse(State s, Action a, StateActionResult &result,
  const PlannerConfig &planner_config)
{

  std::cout << "Entering isValidStateActionPairReverse:" << std::endl;
  printStateNewline(s);
  printActionNewline(a);
  std::cout << std::endl;

  // Declare stance and flight times
  double t_s = a[6];
  double t_f = a[7];

  // Declare states and lengths for upcoming validity and distance checks
  State s_prev = s;
  State s_next;
  result.length = 0;
  result.a_new = a;
  int phase;
  if (t_f == 0) {
    phase = CONNECT_STANCE;
  } else {
    phase = STANCE;
    s[5] = a[4]; // Apply the reverse impulse to this initial state
  }

  double takeoff_height = 0.3;
  double t_f_takeoff = 0;
  State s_takeoff = s;
  for (double t = 0; t < t_f; t += planner_config.KINEMATICS_RES)
  {
    // State s_next = applyFlight(s, -t);
    // result.length += poseDistance(s_next, s_prev);
    // s_prev = s_next;

    // if (isValidState(s_next, planner_config, FLIGHT) == false) {
    //   return false;
    // }

    State s_next = applyFlight(s, -t);
    result.length += poseDistance(s_next, s_prev);
    result.a_new[7] = t;
    result.s_new = s_next;

    std::cout << "s_next: ";
    printStateNewline(s_next);
     
    // If newly within takeoff height, update takeoff state
    if (s_prev[2] - getHeightFromState(s_prev, planner_config) > takeoff_height &&
        s_next[2] - getHeightFromState(s_next, planner_config) <= takeoff_height){
      t_f_takeoff = t;
      s_takeoff = s_next;
      result.s_new = s_takeoff;
      std::cout << "new takeoff height found" <<std::endl;
    }

    // Check if landing collision has been detected
    if (t_f_takeoff > 0) {
      if (!isValidState(s_next, planner_config, STANCE)) {

        // Accept if landing and collision are far enough away, otherwise reject
        if (planner_config.PEAK_GRF_MAX*planner_config.G_CONST > 
            pow(getSpeed(s_takeoff),2)/(2*poseDistance(s_takeoff,s_next)) &&
            isValidState(s_takeoff, planner_config, STANCE)) {

          result.a_new[7] = t_f_takeoff;
          result.s_new = s_takeoff;

          double dzf = s_takeoff[5];
          double zf = s_takeoff[2];
          double z0_ref = 0.2 + getHeightFromState(s_takeoff, planner_config);
          double peak_grf = a[2];
          double g = planner_config.G_CONST;
          t_s = -(3*(dzf + sqrt(pow(dzf,2) - 2*g*z0_ref + 2*g*zf + (4*g*peak_grf*z0_ref)/3.0 -
            (4*g*peak_grf*zf)/3.0)))/(3*g - 2*g*peak_grf);

          t_s = 0.2;
          a[6] = t_s;
          result.a_new[6] = t_s;

          std::cout << "internal: " << pow(dzf,2) - 2*g*(z0_ref + 2*g*zf + (4*g*peak_grf*(z0_ref - zf))/3.0) << std::endl;
          printf("dzf = %6.4f,g = %6.4f,peak_grf = %6.4f,z0_ref = %6.4f,zf = %6.4f\n",
            dzf, g, peak_grf, z0_ref, zf);
          std::cout << "Takeoff is valid, continuing" <<std::endl;
          break;
        } else {
          std::cout << "Takeoff is invalid, exiting" <<std::endl;
          return false;
        }
      }
    }

    // Check collision in flight
    if (!isValidState(s_next, planner_config, FLIGHT)) {
      std::cout << "Invalid flight state, exiting" <<std::endl;
      return false;
    }

    s_prev = s_next;
  }

  std::cout << "t_s = " << t_s << std::endl;
  // State s_takeoff = applyFlight(s, -t_f);

  for (double t = t_s; t >= 0; t -= planner_config.KINEMATICS_RES)
  {

    State s_next = applyStanceReverse(s_takeoff,a,t,planner_config);

    std::cout << "s_next: ";
    printStateNewline(s_next);

    if (isValidState(s_next, planner_config, phase) == false || (!isValidYawRate(s,a,t,planner_config)))
    {
      if (phase == CONNECT_STANCE) {
        t_s = planner_config.BACKUP_RATIO*(t_s - t);
        a[6] = result.t_new;
      }
      // a[6] = result.t_new;
      // result.a_new[6] = result.t_new;
      result.s_new = applyStanceReverse(s_takeoff,a,t_s, planner_config);
      // result.s_new = applyStance(s,a,result.t_new, planner_config);
      std::cout << "Invalid stance state, exiting" <<std::endl;
      return false;
    } else {
      result.t_new = t_s - t;
      result.s_new = s_next;
      // result.a_new[6] = result.t_new;
      result.length += poseDistance(s_next, s_prev);
      s_prev = s_next;
    }
  }

  // Check the exact starting state
  State s_start = applyStanceReverse(s_takeoff,a, planner_config);
  if (isValidState(s_start, planner_config, phase) == false)
  {
    std::cout << "Invalid stance state at start, exiting" <<std::endl;
    return false;
  } else {
    result.t_new = t_s;
    result.s_new = s_start;
    result.length += poseDistance(s_start, s_prev);
  }
  // If both stance and flight trajectories are entirely valid, return true
  std::cout << "Valid action, exiting with " <<std::endl;
  return true;
}

void publishStateActionPair(const State &s, const Action &a, const State &s_goal,
  const PlannerConfig &planner_config, visualization_msgs::MarkerArray &tree_viz_msg,
  ros::Publisher &tree_pub) {

  // Confirm ros is ok
  if (!ros::ok())
    return;

  // Create the marker object for the new state action pair
  visualization_msgs::Marker state_action_line;
  state_action_line.header.stamp = ros::Time::now();
  state_action_line.header.frame_id = "map";
  state_action_line.action = visualization_msgs::Marker::ADD;
  state_action_line.pose.orientation.w = 1;
  state_action_line.id = tree_viz_msg.markers.size();
  state_action_line.type = visualization_msgs::Marker::LINE_STRIP;
  state_action_line.scale.x = 0.06;

  // Interpolate the state action pair
  double t0 = 0;
  double dt = 0.03;
  std::vector<State> interp_state_action;
  std::vector<GRF> interp_grf;
  std::vector<double> interp_t;
  std::vector<int> interp_primitive_id;
  std::vector<double> interp_length;
  interp_length.push_back(0.0);
  interpStateActionPair(s,a,t0,dt,interp_state_action,interp_grf,interp_t,interp_primitive_id,
    interp_length,planner_config);

  // Add the exact last state so the visualization is continuous
  interp_state_action.push_back(applyAction(s,a,planner_config));
  interp_primitive_id.push_back(interp_primitive_id.back());

  // Loop through the interpolated state action pair and add points to the line strip
  int length = interp_state_action.size();
  for (int i=0; i < length; i++) {

    // Load in the pose data from the interpolated path
    geometry_msgs::Point point;
    point.x = interp_state_action.at(i)[0];
    point.y = interp_state_action.at(i)[1];
    point.z = interp_state_action.at(i)[2];

    // Set the color of the line strip according to the motion primitive
    std_msgs::ColorRGBA color;
    color.a = 1;
    if (interp_primitive_id[i] == FLIGHT) {
      color.r = 0/255.0;
      color.g = 45.0/255.0;
      color.b = 144.0/255.0;
    } else if (interp_primitive_id[i] == STANCE) {
      color.r = 0/255.0;
      color.g = 132.0/255.0;
      color.b = 61.0/255.0;
    } else if (interp_primitive_id[i] == CONNECT_STANCE) {
      color.r = 166.0/255.0;
      color.g = 25.0/255.0;
      color.b = 46.0/255.0;
    } else {
      ROS_WARN_THROTTLE(1, "Invalid primitive ID received in planning utils");
    }

    // Add the point and the color
    state_action_line.points.push_back(point);
    state_action_line.colors.push_back(color);  
  }

  // Add the marker to the array
  tree_viz_msg.markers.push_back(state_action_line);

  // Create the marker object for the goal marker of this particular action
  visualization_msgs::Marker goal_marker;
  goal_marker.header.stamp = ros::Time::now();
  goal_marker.header.frame_id = "map";
  goal_marker.action = visualization_msgs::Marker::ADD;
  goal_marker.id = 0;
  goal_marker.type = visualization_msgs::Marker::ARROW;

  // Define the point for the arrow base
  geometry_msgs::Point goal_marker_base;
  goal_marker_base.x = s_goal[0];
  goal_marker_base.y = s_goal[1];
  goal_marker_base.z = s_goal[2];

  // Define the point for the arrow tip and set the scaling accordingly
  geometry_msgs::Point goal_marker_tip;
  double scale = 0.5;
  double vel_magnitude = sqrt(s_goal[3]*s_goal[3] + s_goal[4]*s_goal[4] + s_goal[5]*s_goal[5]);
  goal_marker.scale.x = 0.05*scale*vel_magnitude;
  goal_marker.scale.y = 0.1*scale*vel_magnitude;
  goal_marker_tip.x = s_goal[0] + scale*s_goal[3];
  goal_marker_tip.y = s_goal[1] + scale*s_goal[4];
  goal_marker_tip.z = s_goal[2] + scale*s_goal[5];

  // Add the points to the goal marker and set the color
  goal_marker.points.push_back(goal_marker_base);
  goal_marker.points.push_back(goal_marker_tip);
  goal_marker.color.a = 1.0;
  goal_marker.color.g = 1.0;

  // Add the goal marker to the front of the marker array
  tree_viz_msg.markers.front() = goal_marker;

  // Publish the tree and wait so that RViz has time to process it
  tree_pub.publish(tree_viz_msg);
  double freq = 2.0; // Hz
  usleep(1000000.0/freq);

}

}

