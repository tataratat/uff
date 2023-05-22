program uffomg

  use uff, only: ufffortran
  implicit none

  real(8) :: points(15)
  integer :: npoints = 5
  integer :: pdim = 3
  real(8) :: metric(3)
  real(8) :: tolerance = 1e-5
  real(8) :: newpoints(15)
  integer :: newpointmasks(5)
  integer :: nnewpoints = -1
  integer :: inverse(5)
  integer :: stable = 1

  integer :: i !< counter

  points = (/ 1., 1., 1., &
            & 2., 2., 2., &
            & 1., 1., 1., &
            & 3., 3., 3., &
            & 3., 3., 3. /)

  metric = (/ 1., 0., 0./)
  newpoints(:) = -999999999.
  newpointmasks(:) = 0
  inverse(:) = -1

  call ufffortran(    &
        & points,     &
        & npoints,    &
        & pdim,       &
        & metric,     &
        & tolerance,  &
        & stable,     &
        & newpoints,  &
        & newpointmasks,  &
        & nnewpoints, &
        & inverse     &
  &)

  print *, "points:"
  do i = 1, npoints
    print *, points((i - 1)*pdim + 1:i*pdim)
  end do
  print *, "npoints:", npoints
  print *, "pdim:", pdim
  print *, "metric:"
  print *, metric
  print *, "tolerance:", tolerance
  print *, "newpoints:"
  do i = 1, npoints
    print *, newpoints((i - 1)*pdim + 1:i*pdim)
  end do
  print *, "newpointmasks:", newpointmasks
  print *, "nnewpoints:", nnewpoints
  print *, "inverse:", inverse

end program uffomg
