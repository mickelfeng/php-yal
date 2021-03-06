/*
  +----------------------------------------------------------------------+
  | Yet Another Framework                                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Jun Wen  <wenjun1055@gmail.com>                              |
  +----------------------------------------------------------------------+
*/
  
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

#include "php_yal.h"
#include "yal_acl_acl.h"
#include "yal_acl_acl_interface.h"
#include "yal_acl_role_registery.h"
#include "yal_acl_assertion_assertion_interface.h"
#include "yal_acl_resource_resource_interface.h"

#include "Zend/zend_interfaces.h"

zend_class_entry *yal_acl_acl_ce;

/** {{{ ARG_INFO
*/
ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_add_role_arg, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, role, Yal\\Acl\\AclInterface, 0)
    ZEND_ARG_INFO(0, parents)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_get_role_arg, 0, 0, 1)
    ZEND_ARG_INFO(0, role)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_has_role_arg, 0, 0, 1)
    ZEND_ARG_INFO(0, role)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_inherits_role_arg, 0, 0, 2)
    ZEND_ARG_INFO(0, role)
    ZEND_ARG_INFO(0, inherit)
    ZEND_ARG_INFO(0, onlyParents)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_remove_role_arg, 0, 0, 1)
    ZEND_ARG_INFO(0, role)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_add_resource_arg, 0, 0, 1)
    ZEND_ARG_INFO(0, resource)
    ZEND_ARG_INFO(0, parent)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_get_resource_arg, 0, 0, 1)
    ZEND_ARG_INFO(0, resource)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_has_resource_arg, 0, 0, 1)
    ZEND_ARG_INFO(0, resource)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_inherits_resource_arg, 0, 0, 2)
    ZEND_ARG_INFO(0, resource)
    ZEND_ARG_INFO(0, inherit)
    ZEND_ARG_INFO(0, onlyParents)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_remove_resource_arg, 0, 0, 1)
    ZEND_ARG_INFO(0, resource)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_allow_arg, 0, 0, 0)
    ZEND_ARG_INFO(0, roles)
    ZEND_ARG_INFO(0, resources)
    ZEND_ARG_INFO(0, privileges)
    ZEND_ARG_OBJ_INFO(0, role, Yal\\Acl\\Assertion\\AssertionInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_deny_arg, 0, 0, 0)
    ZEND_ARG_INFO(0, roles)
    ZEND_ARG_INFO(0, resources)
    ZEND_ARG_INFO(0, privileges)
    ZEND_ARG_OBJ_INFO(0, role, Yal\\Acl\\Assertion\\AssertionInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_remove_allow_arg, 0, 0, 0)
    ZEND_ARG_INFO(0, roles)
    ZEND_ARG_INFO(0, resources)
    ZEND_ARG_INFO(0, privileges)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_remove_deny_arg, 0, 0, 0)
    ZEND_ARG_INFO(0, roles)
    ZEND_ARG_INFO(0, resources)
    ZEND_ARG_INFO(0, privileges)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_set_rule_arg, 0, 0, 2)
    ZEND_ARG_INFO(0, operation)
    ZEND_ARG_INFO(0, type)
    ZEND_ARG_INFO(0, roles)
    ZEND_ARG_INFO(0, resources)
    ZEND_ARG_INFO(0, privileges)
    ZEND_ARG_OBJ_INFO(0, role, Yal\\Acl\\Assertion\\AssertionInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_get_child_resources_arg, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, resource, Yal\\Acl\\Resource\\ResourceInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yal_acl_acl_is_allowed_arg, 0, 0, 0)
    ZEND_ARG_INFO(0, role)
    ZEND_ARG_INFO(0, resource)
    ZEND_ARG_INFO(0, privilege)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ proto public Yal\Acl\Acl::__construct(void)
 */
PHP_METHOD(yal_acl_acl, __construct) 
{
    zval *resources, *rules, *array_temp_1, *array_temp_2, *array_temp_3, *temp;
    MAKE_STD_ZVAL(resources);
    MAKE_STD_ZVAL(rules);
    MAKE_STD_ZVAL(array_temp_1);
    MAKE_STD_ZVAL(array_temp_2);
    MAKE_STD_ZVAL(array_temp_3);
    array_init(resources);
    array_init(rules);
    array_init(array_temp_1);
    array_init(array_temp_2);
    array_init(array_temp_3);
    
    add_assoc_zval(rules,          "byResourceId",  resources);
    add_assoc_string(array_temp_3, "type",          YAL_ACL_ACL_PROPERTY_NAME_TYPE_DENY, 1);
    add_assoc_null(array_temp_3,   "assert");
    add_assoc_zval(array_temp_2,   "allPrivileges", array_temp_3);
    add_assoc_zval(array_temp_2,   "byPrivilegeId", resources);
    add_assoc_zval(array_temp_1,   "allRoles",      array_temp_2);
    add_assoc_zval(array_temp_1,   "byRoleId",      resources);
    add_assoc_zval(rules,          "allResources",  array_temp_1);
    
    zend_update_property(yal_acl_acl_ce, getThis(), ZEND_STRL(YAL_ACL_ACL_PROPERTY_NAME_RELES),     rules TSRMLS_CC);
    zend_update_property(yal_acl_acl_ce, getThis(), ZEND_STRL(YAL_ACL_ACL_PROPERTY_NAME_RESOURCES), resources TSRMLS_CC);
    
    zval_ptr_dtor(&resources);
    zval_ptr_dtor(&rules);
    
    return SUCCESS;
}
/* }}} */

/** {{{ proto public Yal\Acl\Acl::getRoleRegistry(void)
 */
PHP_METHOD(yal_acl_acl, getRoleRegistry) 
{
    zval *role_registry, *obj_registery;
    role_registry = zend_read_property(yal_acl_acl_ce, getThis(), ZEND_STRL(YAL_ACL_ACL_PROPERTY_NAME_ROLE_REGISTER), 0 TSRMLS_CC);
    if (Z_TYPE_P(role_registry) == IS_NULL) {
        MAKE_STD_ZVAL(obj_registery);
        object_init_ex(obj_registery, yal_acl_role_registery_ce);
        *role_registry = *obj_registery;
        INIT_PZVAL(role_registry);
        zval_copy_ctor(role_registry);
        zval_ptr_dtor(&obj_registery);
    }
    RETURN_ZVAL(role_registry, 1, 0);
}
/* }}} */

/** {{{ proto public Yal\Acl\Acl::hasResource(void)
 */
PHP_METHOD(yal_acl_acl, hasResource) 
{
    return SUCCESS;
}
/* }}} */

/** {{{ proto public Yal\Acl\Acl::isAllowed(void)
 */
PHP_METHOD(yal_acl_acl, isAllowed) 
{
    return SUCCESS;
}
/* }}} */


/** {{{ yal_acl_acl_methods
 */
zend_function_entry yal_acl_acl_methods[] = {
    PHP_ME(yal_acl_acl, __construct,        NULL,                         ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
    PHP_ME(yal_acl_acl, getRoleRegistry,    NULL,                         ZEND_ACC_PUBLIC)
    PHP_ME(yal_acl_acl, hasResource,        yal_acl_acl_has_resource_arg, ZEND_ACC_PUBLIC)
    PHP_ME(yal_acl_acl, isAllowed,          yal_acl_acl_is_allowed_arg,   ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};
/* }}} */

/** {{{ ZEND_MINIT_FUNCTION
 */
ZEND_MINIT_FUNCTION(yal_acl_acl)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Yal\\Acl\\Acl", yal_acl_acl_methods);
    yal_acl_acl_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);
    //zend_class_implements(yal_acl_acl_ce TSRMLS_CC, 1, yal_acl_acl_interface_ce);
    yal_acl_acl_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;
    
    zend_declare_property_null(yal_acl_acl_ce, ZEND_STRL(YAL_ACL_ACL_PROPERTY_NAME_ROLE_REGISTER),        ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(yal_acl_acl_ce, ZEND_STRL(YAL_ACL_ACL_PROPERTY_NAME_RESOURCES),            ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(yal_acl_acl_ce, ZEND_STRL(YAL_ACL_ACL_PROPERTY_NAME_IS_ALLOWED_ROLE),      ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(yal_acl_acl_ce, ZEND_STRL(YAL_ACL_ACL_PROPERTY_NAME_IS_ALLOWED_RESOURCE),  ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(yal_acl_acl_ce, ZEND_STRL(YAL_ACL_ACL_PROPERTY_NAME_IS_ALLOWED_PRIVILEGE), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(yal_acl_acl_ce, ZEND_STRL(YAL_ACL_ACL_PROPERTY_NAME_RELES),                ZEND_ACC_PROTECTED TSRMLS_CC);
    
    return SUCCESS;
}
/* }}} */
