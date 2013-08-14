api = 2
core = 7.x

; Include the definition of building core directly.
includes[] = "drupal-org-core.make"

; Download the ImageX install profile base and its dependencies from GitHub.
projects[imagex][type] = "profile"
projects[imagex][download][type] = "git"
projects[imagex][download][url] = "git@github.com:imagex/imagex.base.git"
projects[imagex][download][branch] = "7.x-dev"