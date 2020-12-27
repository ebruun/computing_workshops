      program main
      use mpi
      implicit none
      double precision  PI25DT
      parameter        (PI25DT = 3.141592653589793238462643d0)
      double precision  mypi, pi, h, sum, x, f, a
      integer n, myid, numprocs, i, ierr


      call MPI_INIT(ierr)
      call MPI_COMM_RANK(MPI_COMM_WORLD, myid, ierr)
      call MPI_COMM_SIZE(MPI_COMM_WORLD, numprocs, ierr)

      open(12,file='nslices.in',status='old')
      read(12,*) n
      close(12)
      write(*,*)'  number of intervals=',n
c
      h = 1.0d0/n
      sum  = 0.0d0
c  Split the loop between the MPI tasks
      do i = myid*n/numprocs+1, (myid+1)*n/numprocs
         x = h * (dble(i) - 0.5d0)
         sum = sum + 4.d0/(1.d0 + x*x)
      enddo
      mypi = h * sum
c
      pi = 0.
c  Global reduction. All processes send their value of mypi to process 0
c  and process 0 adds them up (MPI_SUM)
      call MPI_REDUCE(mypi,pi,1,MPI_DOUBLE_PRECISION,MPI_SUM,0,
     &                MPI_COMM_WORLD,ierr)

      write(*,*)'myid=',myid,' pi=',pi,'  Error=',abs(pi - PI25DT)

      call MPI_FINALIZE(ierr)

      end
