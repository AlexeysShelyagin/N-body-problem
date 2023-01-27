cd ../cmake-build-debug
N_body_problem.exe -n 2body.json -o 2body.bin -m euler
N_body_problem.exe -n 2body.json -o 2body.bin -m runge_kutta
N_body_problem.exe -n 2body.json -o 2body.bin -m verlet
N_body_problem.exe -n 2body.json -o 2body.bin -m adams_bashforth
N_body_problem.exe -n 2body.json -o 2body.bin -m implicit_runge_kutta
