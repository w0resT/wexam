#pragma once
#include <memory>

#include "../../interfaces/gui/ipageview.h"
#include "../../interfaces/users/istudentrepository.h"
#include "../../interfaces/users/iusermanager.h"
#include "../../interfaces/localization/ilocalizationmanager.h"

class PageUsers : public IPageView {
public:
	PageUsers( std::shared_ptr<ILocalizationManager> _localize, std::shared_ptr<IStudentRepository> _students, std::shared_ptr<IUserManager> _manager ) :
		m_LocalizationManager( std::move( _localize ) ), m_Repository( std::move( _students ) ), m_Manager( std::move( _manager ) ) {}

	void Draw() override;

private:
	std::shared_ptr<IStudentRepository> m_Repository;
	std::shared_ptr<IUserManager> m_Manager;
	std::shared_ptr<ILocalizationManager> m_LocalizationManager;

};