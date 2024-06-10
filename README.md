Compute node에서 연산된 데이터들을 I/O node에서 모으는 두가지 병렬 I/O 방식방식을 각각 다른 통신 수단을 사용하여 실험하고 성능을 비교하는 과제

목적 : 계산 및 I/O 속도를 높이기 위한 병렬 작업(parallel operation)을 구현하기 위함

-Client-oriented collective I/O
각 client는 compute node가 주어진 영역의 데이터들을 다른 client로 부터 모아서 연속적인 데이터들을 보내는 방법

-Server-oriented collective I/O
각 server는 I/O node가 주어진 영역의 데이터들을 client로 부터 모아서 저장하는 방법

조건사항
static library를 사용해서 구현할 것
4processes, 4개의 node file을 사용하여, 전체 data크기는 4MB
두기법의 통신 기법이 달라야 함(pipe, fifo, message queue, shared memory, socke)
각 I/O node에 분산되는 chunk크기는 4KB (1K of integer)
각 I/O node와 compute node에 분산된 data를 파일로 저장할 것
