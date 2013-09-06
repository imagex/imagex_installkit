api = 2
core = 7.x

; Download Drupal core.
projects[drupal][type] = "core"
projects[drupal][version] = "7.23"

; If patches are to be applied, apply them below by using projects[drupal][patch][].
; Ensure that each patch is well documented here as to why it is being applied.

; Deprecated patch for core that allows only a single profile to be inherited.
; This patch is no longer used in favor of the multi-inheritable profiles patch.
; projects[drupal][patch][] = https://gist.github.com/amcgowanca/6191652/raw/53bb5248777f07a5c865cfc1f1e32ad3b0bf9392/1356276-D7-inheritable-profiles
projects[drupal][patch][] = https://gist.github.com/amcgowanca/6444553/raw/e8d840836ad76adc99f92f294b288431710e6437/1356276-D7-inheritable-profiles-multi