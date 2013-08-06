api = 2
core = 7.x

; Include the definition of building core directly.
includes[] = "drupal-org-core.make"

; Download the ImageX install profile base and its dependencies from GitHub.
projects[ixm][type] = "profile"
projects[ixm][download][type] = "git"
projects[ixm][download][url] = "git://github.com:imagex/ixm-base.git"
projects[ixm][download][branch] = "7.x-dev"
