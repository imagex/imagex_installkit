api = 2
core = 7.x

; Download Drupal core.
projects[drupal][type] = "core"
projects[drupal][version] = "7.26"
; projects[drupal][download][type] = git
; projects[drupal][download][tag] = "7.26"
; projects[drupal][download][url] = http://git.drupal.org/project/drupal.git

; If patches are to be applied, apply them below by using projects[drupal][patch][].
; Ensure that each patch is well documented here as to why it is being applied.
; Enables multi-profile inheritance with deep dependency checking.
projects[drupal][patch][] = "https://raw.github.com/imagex/imagex_patches/7.x/core/inheritable-profiles/1356276-D7-inhertiable-profiles-multi-enforce-dependencies.patch"
; Resolves the undefined tab_root_map variable in menu translate problems.
projects[drupal][patch][] = "https://raw.github.com/imagex/imagex_patches/7.x/core/undefined-menu-translate-notice-951098-50.patch"
; Resolves file entity field properties being overwritten.
projects[drupal][patch][] = "https://raw.github.com/imagex/imagex_patches/7.x/core/2066275-file-field-load-merge-order.patch"
