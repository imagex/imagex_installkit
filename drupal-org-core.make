api = 2
core = 7.x

; Download Drupal core.
projects[drupal][type] = "core"
projects[drupal][version] = "7.22"

; If patches are to be applied, apply them below by using projects[drupal][patch][].
; Ensure that each patch is well documented here as to why it is being applied.

; projects[drupal][patch][] = https://drupal.org/files/1356276-D7-inheritable-profiles.patch
projects[drupal][patch][] = https://gist.github.com/amcgowanca/6191652/raw/53bb5248777f07a5c865cfc1f1e32ad3b0bf9392/1356276-D7-inheritable-profiles