!> uff module, in case one prefers to have `use uff`
module uff
  implicit none

contains
  subroutine ufffortran( &
      & points,          &
      & npoints,         &
      & pdim,            &
      & metric,          &
      & tolerance,       &
      & stable,          &
      & newpoints,       &
      & newpointmasks,   &
      & nnewpoints,      &
      & inverse)

    real(8), intent(in) :: points(:)
    integer, intent(in) :: npoints
    integer, intent(in) :: pdim
    real(8), intent(in) :: metric(:)
    real(8), intent(in) :: tolerance
    integer, intent(in) :: stable
    real(8), intent(out) :: newpoints(:)
    integer, intent(out) :: newpointmasks(:)
    integer, intent(out) :: nnewpoints
    integer, intent(out) :: inverse(:)

    call ufff(           &
        & points,        &
        & npoints,       &
        & pdim,          &
        & metric,        &
        & tolerance,     &
        & stable,     &
        & newpoints,     &
        & newpointmasks, &
        & nnewpoints,    &
        & inverse)

    end subroutine ufffortran


end module
