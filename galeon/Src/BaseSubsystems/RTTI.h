#ifndef RTTI_H_
#define RTTI_H_

#include <string>

class RTTI 
{
  public:
    RTTI(const std::string & className);
    RTTI(const std::string & className, const RTTI & baseRTTI);
    RTTI(const std::string & className, int numParents, ...);
    ~RTTI();

    const std::string & GetClassName() const { return m_className; }
    bool IsExactly(const RTTI & rtti) const { return (this == &rtti); }
    bool DerivesFrom (const RTTI & rtti) const;

  private:
    // Prevent copying
    RTTI( const RTTI & obj );
    RTTI & operator=( const RTTI & obj );

    const std::string m_className;

    int           m_numParents;
    const RTTI ** m_pBaseRTTI;
};


#define RTTI_DECL \
    public: \
        virtual const RTTI & GetRTTI() { return rtti; } \
        static const RTTI rtti;

// Versión de RTTI_DECL para clases con template: Añade declaración de parámetro de plantilla y quita el virtual porque da conflicto
#define RTTI_DECL_TEMPLATE(templateClassName) \
	public: \
		template <class templateClassName> \
        const RTTI & GetRTTI() { return rtti; } \
        static const RTTI rtti;

#define RTTI_ROOT_IMPL(name) \
    const RTTI name::rtti(#name);

#define RTTI_IMPL(name,parent) \
    const RTTI name::rtti(#name, parent::rtti);

#define RTTI_IMPL_TEMPLATE(name,parent,templateClassName) \
	template <class templateClassName> \
    const RTTI name<templateClassName>::rtti(#name, parent::rtti);


#endif  // _RTTI_H_

