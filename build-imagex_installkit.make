api = 2
core = 7.x

; Download Drupal core.
includes[] = "https://raw.github.com/imagex/imagex_installkit/7.x-1.x/drupal-org-core.make"

; Download the ImageX base installation profile.
projects[imagex_installkit][type] = "profile"
projects[imagex_installkit][download][type] = "git"
projects[imagex_installkit][download][url] = "git@github.com:imagex/imagex_installkit.git"
projects[imagex_installkit][download][branch] = "7.x-1.x"
