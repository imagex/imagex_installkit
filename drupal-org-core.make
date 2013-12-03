api = 2
core = 7.x

; Download Drupal core.
projects[drupal][type] = "core"
projects[drupal][version] = "7.23"
projects[drupal][download][type] = git
projects[drupal][download][tag] = "7.23"
projects[drupal][download][url] = http://git.drupal.org/project/drupal.git

; If patches are to be applied, apply them below by using projects[drupal][patch][].
; Ensure that each patch is well documented here as to why it is being applied.

; Deprecated patch for core that allows only a single profile to be inherited.
; This patch is no longer used in favor of the multi-inheritable profiles patch.
projects[drupal][patch][] = "https://raw.github.com/imagex/imagex_patches/7.x/core/inheritable-profiles/1356276-D7-inhertiable-profiles-multi-enforce-dependencies.patch"

; Resolves the undefined tab_root_map variable in menu translate problems.
projects[drupal][patch][] = "https://raw.github.com/imagex/imagex_patches/7.x/core/undefined-menu-translate-notice-951098-50.patch"